#ifndef GUI_MAIN_WINDOW_CONTROL_PANEL_HPP
#define GUI_MAIN_WINDOW_CONTROL_PANEL_HPP

#include <memory>
#include <utility>

#include <QFrame>
#include <QWidget>

// Include Qt generated files
//
#include "gui/ui_main_window_control_panel.h"

namespace Ui {
    class MainWindowControlPanel;
} // namespace Ui

class MainWindowControlPanel : public QFrame {
    Q_OBJECT

public:
    enum class Message {
        kEmpty,
        kUnknownErrorOccurred,
        kInvalidKeyEntered,
        kKeyExists,
        kKeyNotExists,
        kNodeAdded,
        kNodeRemoved,
    };

    explicit MainWindowControlPanel(QWidget* parent = nullptr);

    void UpdateMessage(Message mes);

signals:
    void AddNode(int key);
    void RemoveNode(int key);

private:
    void AddButtonClicked();
    void RemoveButtonClicked();
    std::pair<unsigned int, bool> ReadNodeKey();

    const int kNodeKeyValueLeftBound = 1;
    const int kNodeKeyValueRightBound = 999;

    std::unique_ptr<Ui::MainWindowControlPanel> ui_;
};

#endif // !GUI_MAIN_WINDOW_CONTROL_PANEL_HPP

