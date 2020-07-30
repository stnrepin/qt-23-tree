#ifndef GUI_TWO_THREE_TREE_CANVAS_HPP
#define GUI_TWO_THREE_TREE_CANVAS_HPP

#include <memory>
#include <queue>
#include <stack>

#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QResizeEvent>
#include <QWidget>

#include "2-3_tree_impl/2-3_tree.hpp"

// Include Qt generated files.
//
#include "gui/ui_two_three_tree_canvas.h"

namespace Ui {
    class TwoThreeTreeCanvas;
} // namespace Ui

struct TwoThreeTreeCanvasOptions {
    const int node_ellipse_radius = 20;
    const int node_key_font_size = 16;
    const int pen_size = 4;
    const int canvas_padding = 40;
};

class TwoThreeTreeCanvas : public QWidget {
    Q_OBJECT

public:
    TwoThreeTreeCanvas(const two_three_tree::TwoThreeTree* tree,
                                const TwoThreeTreeCanvasOptions& opt,
                                QWidget* parent = nullptr);

    void Clear();

    void PrintTree();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    std::pair<float, float>
        CalculateNodePosition(int index, int depth, int tree_depth) const;

    std::unique_ptr<Ui::TwoThreeTreeCanvas> ui_;
    QPixmap pm_;
    const TwoThreeTreeCanvasOptions opt_;
    const two_three_tree::TwoThreeTree* tree_;
};

#endif // !GUI_TWO_THREE_TREE_CANVAS_HPP

