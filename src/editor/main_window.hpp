#pragma once

#include "undo_base.hpp"
#include "select/selection.hpp"
#include "image/model_renderer.hpp"
#include "image/image_view.hpp"
#include "widget_stack.hpp"
#include "color/effect_types.hpp"
#include "select/selector_types.hpp"

#include <QWidget>
#include <QCloseEvent>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QButtonGroup>
#include <QFileInfo>
#include <QDir>

namespace editor {

/* stores a change to the image in the editor, selections are tied to images
 * a change either moves between images and selections, or changes what's been
 * selected
 * each selection stores its own undo list, so we have a nesting effect
 * going on, main_window declares its own un/redo functions because of this
 */
struct editor_change {
	// whether img came from open_image() or was altered from a previous image
	enum change_type {initial, altered} c_type;
	// the original image for this change
	QImage img;
	// the selection of points on the image, stores an undo list and is heavily
	// connected to main_window and its member variables
	// TODO memory leak: selections aren't getting deleted when changes are cleared
	// but when main_window is destroyed because main_window is their QObject parent
	// technically not undefined behavior, but not the behavior I want
	select::selection *sel;
}; // editor_change

/* 
 * The main_window class, it does things that require full knowledge of every
 * component in the color_editor, these are listed below. main_window inherits
 * from QWidget because GUI, and undo_base for undoing
 * - holds the overall GUI layout, including a rudimentary menubar
 * - holds a pointer the current selection and the current source image, both of
 *   which are also stored as references in the model renderer
 * - creates and facilitates the connections between the renderer, selection,
 *   image view, selectors, and color effect components
 * - manages undoing and the nested undoing of individual selections
 * - handles image file IO and keeps track of the state of image modifications
 * - manages keybindings for undo and redo
 * 
 * an overview of the general flow of the program is in the comment above
 * make_major_connections()
 */
class main_window : public QWidget, public undo_base<editor_change> {
	Q_OBJECT
	
public:
	/* calls three private functions:
	 * initialize_members(), make_major_connections(), and setup_layout()
	 * see each function below for details
	 * 
	 * currently also attempts to open mantis300.jpg for ease of testing
	 */
	main_window(QWidget *parent = nullptr);
	
public slots:
	/* handles the nesting of un/redo inside selections that are inside
	 * editor_change objects
	 * specifically, will do an undo or redo in cur_selection before doing
	 * an editor_change un/redo
	 * does editor_change undo/redo through undo_base
	 */
	void undo();
	void redo();
	
	/* calls future_changes.clear() that's it
	 * the reason this is here is so that I can connect and disconnect it to
	 * the current selection, you can't disconnect lambdas
	 */
	void clear_undone();
	
	/* these handle the opening and closing of images, and are connected to a
	 * rudimentary menubar
	 * they check the current state of the image, and open a pop-up if there are
	 * unsaved modifications, if there aren't or the user discards the changes,
	 * they either open a new image from a file or save the current image to a file
	 * 
	 * these use Qt's built in file dialog systems to handle the file IO
	 * 
	 * TODO make a better menubar using Qt classes
	 */
	void open_image(QString filepath = QString());
	void save_as();
	
protected:
	/* checks for unsaved modifications, closes if none, opens pop-up otherwise
	 */
	void closeEvent(QCloseEvent *event) override;
	/* creates common un/redo keybinds (ctrl+z ctrl+shift+z ctrl+y)
	 */
	// TODO make more keybinds for other stuff
	// TODO add keybinds to new menubar
	void keyPressEvent(QKeyEvent *event) override;
	
private:
	/* the selection at undo_base.past_changes.top()
	 * the selection holds its own undo stack and is very interconnected with
	 * main_window and its other member variables
	 * renderer will hold a reference to this pointer when initialized
	 */
	select::selection *cur_selection = nullptr;
	/* the current source image to be painted on
	 * probably the image at undo_base.past_changes.top() but honestly I didn't
	 * design it that way and haven't checked that that invariant holds
	 * I did design cur_selection that way
	 * renderer will hold a reference to this image when initialized
	 */
	QImage source_img;
	/* QGraphicsItem held inside the image_view, renders the image with the
	 * current effect applied to it
	 * also a QObject for slot connections
	 * holds a reference to cur_selection and source_img, holds the only copies
	 * of the current effect and the effect_state
	 */
	image::model_renderer *renderer;
	/* inherits from QGraphicsView to hold renderer and allow zooming
	 * also handles all mouse input and selection of points via connections to
	 * the selectors and cur_selection
	 * holds renderer and is called for some redraw events because of efficiency
	 */
	image::image_view *view;
	/* communicates the mouse mode from various checkboxes to view, and makes
	 * the checkboxes update with one another
	 * the checkboxes are found in the menubar and in some selectors
	 */
	QButtonGroup *mouse_mode_group;
	/* selectors, these commnicate with view when points are selected to create 
	 * selection regions, they can also spontaneously select regions
	 * the widget stack holds all of them, but only the active() one can select
	 */
	widget_stack<select::selector *> *selector_stack;
	/* color effect widgets, these handle the GUI portion of color effects, they
	 * emit a single altered when their state updates, this gets passed to the
	 * renderer who handles actually creating the effects from the effect_state
	 */
	widget_stack<color::effect_widget *> *effect_stack;
	/* calls store_current_effect() when clicked, see below
	 * not in the menubar, so not created in create_menubar_buttons()
	 */
	QToolButton *apply_b;
	
	// which file was previously open, starts at the home directory 
	// is referenced by open_image and save_as to know where to open the dialog
	QFileInfo previous_file = QDir::homePath();
	// the initial image found at previous_file.filePath()
	// replaced on open_image() and used to check for modifications
	QImage initial_image;
	
	/* creates every member listed above that isn't default constructed
	 * creates and adds selectors and color effects to their widget stacks
	 * adds the renderer to the image_view's scene
	 */
	void initialize_members();
	/* this function makes one-time connections, mostly between things that
	 * change state, and whose state changes require updates to other things
	 * very vague, yes, but honestly its decently somewhat descriptive
	 * 
	 * because connections in general are fundamental to understanding the flow
	 * of the program, I'm going to outline the intended flow here:
	 * 
	 * 1. things get initialized, main_window creates all the member variables,
	 *    places them in the right GUI locations, and makes the connections
	 *    in make_major_connections()
	 * 2. the user opens an image, the image is stored in the renderer, an
	 *    empty selection object is created, and main_window connects it to
	 *    everything else via connect_selection()
	 * 3. the user makes a selection by interacting with a selector, this can be
	 *    by drawing on the image or by clicking a button, or smth else
	 *    this updates the selection and causes a redraw based on the color_effect
	 * 4. at any time the user can edit the color_effect and cause a live redraw
	 *    of the image
	 * TODO make a way to store the state of color_effects and easily swap
	 * between them
	 * 5. when the user is satisfied with the selection and color effect, they
	 *    press the "save effect to image" button (apply_b) that stores the
	 *    effect on the image and creates a new selection, this creates a new
	 *    selection in the  undo stack, which requires disconnecting the old
	 *    selection and connecting the new selection
	 * 6. also going on is the fact that there are a few different types of
	 *    selectors and color effects, these necessitate widget_stacks to manage
	 *    which one is active and which aren't
	 * 
	 * list of connections:
	 * mouse_mode_group idToggled -> view maybe_set_mouse_mode
	 * for each selector:
	 *   view point_selected -> selector point_selected
	 * for each color effect:
	 *   effect altered -> renderer update_effect_state
	 * apply_b clicked -> main_window store_current_effect
	 */
	void make_major_connections();
	
	/* the current selection is a changing thing, but still needs to be connected
	 * it connects to some main_window functions and a number of member variables
	 * these functions control those connections
	 * 
	 * list of connections:
	 * selection contents_updated -> view redraw_rect
	 * view combine_points -> selection combine_changes
	 * for each selector:
	 *   selector region_selected -> selection add_region
	 * selection region_added -> main_window clear_undone
	 * selection boundary_updated -> renderer create_effect_and_render
	 */
	void connect_selection(select::selection *sel);
	void disconnect_selection(select::selection *sel);
	
	/* sets up the layout of the GUI, includes menubar buttons
	 * must be called after items are initialized and after connections made
	 * due to first time show events sending signals
	 */
	void setup_layout();
	/* just separates a lot of code that creates the menubar buttons and their
	 * connections from setup_layout()
	 */
	QHBoxLayout *create_menubar_buttons();
	
	/* initialize_image and store_current_effect are the only functions that
	 * call add() and, therefore, apply()
	 * 
	 * initialize_image creates an editor_change with initial, the input image,
	 * and a new selection
	 * will clear both past and future undo stacks completely
	 * initializes the image no matter what
	 * 
	 * store_current_effect creates an editor_change with altered, the source
	 * from renderer, and a new selection
	 * will clear future_changes in cur_selection, but doesn't touch main_window
	 * won't store the effect if there is no_effective_effect() in renderer
	 * 
	 * see apply() for the effects of calling add() and via undo_base, apply()
	 */
	void initialize_image(const QImage &image);
	void store_current_effect();
	
	/* these do or undo a full editor_change object by switching to the image
	 * and setting the current selection
	 * apply will also update the future editor_change's source image to match 
	 * the current image, so that changes propagate to the future indefinitely
	 */
	void apply(const editor_change &chng) override;
	void unapply(const editor_change &chng) override;
	/* changes the current image in the renderer and causes a redraw
	 * only called in apply() and unapply()
	 */
	void set_image(const QImage &image);
	
	// called in main_window::undo/redo based on cur_selection undo/redo state
	using undo_base<editor_change>::undo;
	using undo_base<editor_change>::redo;
	
	/* returns true if the image has no modifications, if it does have any, opens
	 * a warning box and asks the user if they'd like to save, discard, or cancel
	 * save calls save_as then returns true
	 * discard returns true
	 * cancel returns false
	 */
	bool modifications_resolved();
}; // main_window

} // editor
