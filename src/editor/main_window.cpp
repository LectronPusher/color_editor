#include "main_window.hpp"
#include "mouse_mode.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QLabel>

using namespace editor;

main_window::main_window(QWidget *parent) : QWidget(parent) {
	setWindowTitle("color editor");
	
	initialize_members();
	make_major_connections();
	setup_layout();
	
	// default so I don't have to manually load an image every time
	open_image("../data/mantis300.jpg");
}

static QToolButton *tool_button_text(const QString &text) {
	auto button = new QToolButton;
	button->setText(text);
	return button;
}

void main_window::initialize_members() {
	renderer = new image::model_renderer(cur_selection/*makes a reference*/);
	
	view = new image::image_view;
	view->scene()->addItem(renderer);
	
	mouse_mode_group = new QButtonGroup(this);
	
	selector_stack = new widget_stack<select::selector *>;
	selector_stack->add(new select::selector_types::select_all);
	selector_stack->add(new select::selector_types::draw);
	selector_stack->add(new select::selector_types::color_match);
	for (auto selector : *selector_stack)
		selector->add_checkboxes_to_group(mouse_mode_group);
	
	effect_stack = new widget_stack<color::effect *>;
	effect_stack->add(new color::effect_types::single_color);
	effect_stack->add(new color::effect_types::gradient);
	effect_stack->add(new color::effect_types::pixellate);
	
	apply_b = tool_button_text("Save Effect to Image");
}

void main_window::make_major_connections() {
	connect(mouse_mode_group, &QButtonGroup::idToggled,
			view, &image::image_view::maybe_set_mouse_mode);
	for (auto selector : *selector_stack) {
		connect(view, &image::image_view::point_selected,
				selector, &select::selector::point_selected);
	}
	for (auto effect : *effect_stack) {
		connect(effect, &color::effect::altered,
				this, &main_window::update_effect);
		connect(effect, &color::effect::mode_changed,
				this, [=](painting_mode::mode m){ renderer->update_mode(m); });
	}
	connect(apply_b, &QToolButton::clicked,
			this, &main_window::store_current_effect);
}

void main_window::main_window::connect_selection(select::selection *sel) {
	connect(sel, &select::selection::contents_updated,
			view, &image::image_view::redraw_rect);
	connect(view, &image::image_view::combine_points,
			sel, &select::selection::combine_changes);
	for (auto selector : *selector_stack) {
		connect(selector, &select::selector::region_selected,
				sel, &select::selection::add_region);
	}
	connect(sel, &select::selection::region_added,
			this, &main_window::clear_undone);
	connect(sel, &select::selection::boundary_updated,
			this, &main_window::update_effect);
}

void main_window::main_window::disconnect_selection(select::selection *sel) {
	disconnect(sel, &select::selection::contents_updated,
			view, &image::image_view::redraw_rect);
	disconnect(view, &image::image_view::combine_points,
			sel, &select::selection::combine_changes);
	for (auto selector : *selector_stack) {
		disconnect(selector, &select::selector::region_selected,
				sel, &select::selection::add_region);
	}
	disconnect(sel, &select::selection::region_added,
			   this, &main_window::clear_undone);
	disconnect(sel, &select::selection::boundary_updated,
			this, &main_window::update_effect);
}

void main_window::update_effect() {
	if (cur_selection) {
		renderer->update_effect(
			effect_stack->active()->create_effect(
				renderer->source,
				cur_selection->region_rect()
			)
		);
	}
}

void main_window::clear_undone() {
	// distinct function necessary to call disconnect()
	future_changes.clear();
}

void main_window::initialize_image(const QImage &image) {
	initial_image = image.convertToFormat(QImage::Format_ARGB32);
	past_changes.clear();
	future_changes.clear();
	add({editor_change::initial, initial_image, new select::selection(this)});
}

void main_window::store_current_effect() {
	if (cur_selection) cur_selection->future_changes.clear();
	if (!renderer->no_effective_effect())
		add({editor_change::stored, renderer->source, new select::selection(this)});
}

void main_window::undo() {
	// nothing to do if selection == nullptr
	// note: calling selection->undo has the side effect of undoing
	if (cur_selection != nullptr && !cur_selection->undo())
		undo_base<editor_change>::undo();
}

void main_window::redo() {
	// still valid if selection nullptr
	// note: calling selection->redo has the side effect of redoing
	if (cur_selection == nullptr || !cur_selection->redo())
		undo_base<editor_change>::redo();
}

void main_window::apply(const editor_change &chng) {
	switch (chng.c_type) {
		case editor_change::initial:
			set_image(chng.orig);
			view->setSceneRect(chng.orig.rect());
			view->scale_down_to_fit(chng.orig.rect());
			break;
		case editor_change::stored:
			{
				QPainter painter(&renderer->source);
				renderer->render_effect(&painter, false);
			}
			set_image(renderer->source);
			// if we have a future item, update it with the current image
			if (!future_changes.empty())
				future_changes.top().orig = renderer->source;
	}
	if (cur_selection)
		disconnect_selection(cur_selection);
	cur_selection = chng.sel;
	connect_selection(cur_selection);
	update_effect();
}

void main_window::unapply(const editor_change &chng) {
	switch (chng.c_type) {
		case editor_change::initial:
			// set state back to earlier
			past_changes.push(std::move(future_changes.top()));
			future_changes.pop();
			// quit before undoing, not valid to undo the initial image
			return;
			// return, not break
		case editor_change::stored:
			set_image(chng.orig);
	}
	disconnect_selection(cur_selection);
	cur_selection = past_changes.top().sel;
	connect_selection(cur_selection);
	update_effect();
}

void main_window::set_image(const QImage &image) {
	renderer->source = image;
	select::selector::set_image(image);
	view->redraw_rect(image.rect());
}

static QLabel *horizontal_line() {
	auto hline = new QLabel;
	hline->setFrameStyle(QFrame::HLine);
	hline->setFixedHeight(5);
	hline->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
	return hline;
}

void main_window::setup_layout() {
	auto image_panel = new QVBoxLayout;
	QHBoxLayout *image_buttons = create_image_buttons();
	image_panel->addLayout(image_buttons);
	image_panel->setAlignment(image_buttons, Qt::AlignLeft);
	image_panel->addWidget(view);
	
	QWidget *tool_panel_wrapper = new QWidget;
	auto tool_panel = new QVBoxLayout(tool_panel_wrapper);
	tool_panel->setContentsMargins(0, 0, 0, 0);
	tool_panel->addWidget(selector_stack);
	tool_panel->addWidget(horizontal_line());
	tool_panel->addWidget(effect_stack);
	tool_panel->addWidget(apply_b);
	tool_panel->setAlignment(apply_b, Qt::AlignCenter);
	// requires tool_panel_wrapper, doesn't work without it
	tool_panel->setSizeConstraint(QLayout::SetFixedSize);
	
	auto all_panels = new QHBoxLayout;
	all_panels->addLayout(image_panel);
	all_panels->addWidget(tool_panel_wrapper);
	all_panels->setAlignment(tool_panel_wrapper, Qt::AlignTop);
	setLayout(all_panels);
}

QHBoxLayout *main_window::create_image_buttons() {
	auto open_b = tool_button_text("Open");
	auto save_as_b = tool_button_text("Save As");
	auto undo_b = tool_button_text("Undo");
	auto redo_b = tool_button_text("Redo");
	auto zoom_in_b = tool_button_text("Zoom In");
	auto zoom_out_b = tool_button_text("Zoom Out");
	auto reset_zoom_b = tool_button_text("100%");
	
	connect(open_b, &QToolButton::clicked, this, [=](){ open_image(); });
	connect(save_as_b, &QToolButton::clicked, this, &main_window::save_as);
	connect(undo_b, &QToolButton::clicked, this, &main_window::undo);
	connect(redo_b, &QToolButton::clicked, this, &main_window::redo);
	connect(zoom_in_b, &QToolButton::clicked, view, &image::image_view::zoom_in);
	connect(zoom_out_b, &QToolButton::clicked, view, &image::image_view::zoom_out);
	connect(reset_zoom_b, &QToolButton::clicked, view, &image::image_view::reset_zoom);
	
	auto pan_cb = new QCheckBox("Pan");
	mouse_mode_group->addButton(pan_cb, mouse_mode::pan);
	
	auto image_buttons = new QHBoxLayout;
	image_buttons->addWidget(open_b);
	image_buttons->addWidget(save_as_b);
	image_buttons->addWidget(undo_b);
	image_buttons->addWidget(redo_b);
	image_buttons->addWidget(zoom_in_b);
	image_buttons->addWidget(zoom_out_b);
	image_buttons->addWidget(reset_zoom_b);
	image_buttons->addWidget(pan_cb);
	
	return image_buttons;
}

void main_window::open_image(QString filepath) {
	if (!modifications_resolved()) return;
	if (filepath.isEmpty()) {
		filepath = QFileDialog::getOpenFileName(
			this,
			"Open Image",
			previous_file.dir().path(),
			"Image Files (*.png *.jpg *.bmp)"
		);
	}
	if (!filepath.isEmpty()) {
		previous_file = QFileInfo(filepath);
		QImage image(filepath);
		if (!image.isNull())
			initialize_image(image);
	}
}

void main_window::save_as() {
	if (renderer->source.isNull()) return;
	QString filepath = QFileDialog::getSaveFileName(
		this,
		"Save As",
		previous_file.filePath(),
		"Image Files (*.png *.jpg *.bmp);;All Files (*)"
	);
	if (!filepath.isEmpty()) {
		store_current_effect();
		renderer->source.save(filepath);
		previous_file = QFileInfo(filepath);
	}
}

bool main_window::modifications_resolved() {
	// whether the image was modified
	if (!renderer->no_effective_effect() || renderer->source != initial_image) {
		auto dialog_ans = QMessageBox::warning(
			this,
			"Unsaved Changes",
			"The image has been modified.\nDo you want to save your changes?",
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
		);
		if (dialog_ans == QMessageBox::Cancel)
			return false;
		else if (dialog_ans == QMessageBox::Save)
			save_as();
	}
	return true;
}

void main_window::closeEvent(QCloseEvent *event) {
	if (modifications_resolved())
		event->accept();
	else
		event->ignore();
}

void main_window::keyPressEvent(QKeyEvent *event) {
	auto mods = event->modifiers();
	switch (event->key()) {
		case Qt::Key_Z:
			if (mods == Qt::ControlModifier)
				undo();
			else if (mods == (Qt::ControlModifier | Qt::ShiftModifier))
				redo();
			break;
		case Qt::Key_Y:
			if (mods == Qt::ControlModifier)
				redo();
			break;
		case Qt::Key_Undo:
			undo();
			break;
		case Qt::Key_Redo:
			redo();
			break;
		default:
			break;
	}
}
