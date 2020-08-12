#include "main_window.hpp"
#include "color/effect_types.hpp"
#include "select/selector_types.hpp"
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
	selection = new select::selection(this);
	
	renderer = new image::model_renderer(selection);
	
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
	// view
	connect(selection, &select::selection::contents_updated,
			view, &image::image_view::redraw_rect);
	connect(view, &image::image_view::combine_points,
			selection, &select::selection::combine_changes);
	connect(mouse_mode_group, &QButtonGroup::idToggled,
			view, &image::image_view::maybe_set_mouse_mode);
	// selector
	for (auto selector : *selector_stack) {
		connect(selector, &select::selector::region_selected,
				selection, &select::selection::add_region);
		connect(view, &image::image_view::point_selected,
				selector, &select::selector::point_selected);
	}
	// effect
	connect(selection, &select::selection::boundary_updated,
			this, &main_window::update_effect);
	for (auto effect : *effect_stack) {
		connect(effect, &color::effect::altered,
				this, &main_window::update_effect);
		connect(effect, &color::effect::mode_changed,
				this, [=](painting_mode::mode m){ renderer->update_mode(m); });
	}
	// apply_b
	connect(apply_b, &QToolButton::clicked, this, &main_window::apply_effect);
}

void main_window::update_effect() {
	if (selection) {
		renderer->update_effect(
			effect_stack->active()->create_effect(
				renderer->source,
				selection->region_rect()
			)
		);
	}
}

void main_window::set_image(const QImage &image) {
	renderer->source = image;
	// TODO break selection undo redo
	select::selector::set_image(image);
	update_effect();
	const QRect &rect = image.rect();
	view->setSceneRect(rect);
	view->scale_down_to_fit(rect);
	view->redraw_rect(rect);
}

void main_window::apply_effect() {
	QImage image = renderer->source;
	QPainter painter(&image);
	renderer->render_effect(&painter, false);
	set_image(image);
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
	connect(undo_b, &QToolButton::clicked, this, [=](){ selection->undo(); });
	connect(redo_b, &QToolButton::clicked, this, [=](){ selection->redo(); });
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
	if (modifications_resolved()) {
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
				set_image(image.convertToFormat(QImage::Format_ARGB32));
		}
	}
}

void main_window::save_as() {
	if (!renderer->source.isNull()) {
		QString filepath = QFileDialog::getSaveFileName(
			this,
			"Save As",
			previous_file.filePath(),
			"Image Files (*.png *.jpg *.bmp);;All Files (*)"
		);
		if (!filepath.isEmpty()) {
			apply_effect();
			renderer->source.save(filepath);
			previous_file = QFileInfo(filepath);
		}
	}
}

bool main_window::modifications_resolved() {
	if (
		!renderer->no_effective_effect() ||
		renderer->source != QImage(previous_file.filePath())
	) {
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
				selection->undo();
			else if (mods == (Qt::ControlModifier | Qt::ShiftModifier))
				selection->redo();
			break;
		case Qt::Key_Y:
			if (mods == Qt::ControlModifier)
				selection->redo();
			break;
		case Qt::Key_Undo:
			selection->undo();
			break;
		case Qt::Key_Redo:
			selection->redo();
			break;
		default:
			break;
	}
}
