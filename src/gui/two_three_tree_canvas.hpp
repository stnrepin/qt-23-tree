#ifndef GUI_TWO_THREE_TREE_CANVAS_HPP
#define GUI_TWO_THREE_TREE_CANVAS_HPP

#include <memory>
#include <queue>
#include <stack>

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QResizeEvent>

#include "2-3_tree_impl/2-3_tree.hpp"

// Include Qt generated files.
//
#include "gui/ui_two_three_tree_canvas.h"

namespace Ui {
    class TwoThreeTreeCanvas;
}

struct TwoThreeTreeCanvasOptions {
    int node_ellipse_radius = 20;
    int node_key_font_size = 16;
    int pen_size = 4;
    int canvas_padding = 40;
};

class TwoThreeTreeCanvas : public QWidget {
    Q_OBJECT

public:
    TwoThreeTreeCanvas(const two_three_tree::TwoThreeTree* tree,
                                const TwoThreeTreeCanvasOptions& opt,
                                QWidget* parent = nullptr);
    ~TwoThreeTreeCanvas() noexcept = default;

    void Clear();

    void PrintTree();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void paintEvent(QPaintEvent *event) override;

private:
    std::pair<float, float>
        CalculateNodePosition(int index, int depth, int tree_depth) const;

    std::unique_ptr<Ui::TwoThreeTreeCanvas> ui_;
    QPixmap pm_;
    const TwoThreeTreeCanvasOptions opt_;
    const two_three_tree::TwoThreeTree* tree_;
};

#endif // !GUI_TWO_THREE_TREE_CANVAS_HPP

