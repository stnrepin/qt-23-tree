#include "gui/two_three_tree_canvas.hpp"

TwoThreeTreeCanvas::TwoThreeTreeCanvas(QWidget* parent)
    : QWidget(parent), ui_(std::make_unique<Ui::TwoThreeTreeCanvas>())
{
    ui_->setupUi(this);

    painter_ = new QPainter(this);
}

TwoThreeTreeCanvas::~TwoThreeTreeCanvas() {
    delete painter_;
}
