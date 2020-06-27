#include "selector.hpp"

namespace editor {
namespace select {

selector::selector(QWidget *parent, QString name_in)
: QWidget(parent), name(name_in){
	options = new QGridLayout;
	setLayout(options);
}

} // select
} // editor
