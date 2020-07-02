#include "gui/main_window_control_panel.hpp"

MainWindowControlPanel::MainWindowControlPanel(QWidget* parent)
    : QFrame(parent),
      ui_(std::make_unique<Ui::MainWindowControlPanel>())
{
    ui_->setupUi(this);

    connect(ui_->add_button, &QPushButton::clicked, this, &MainWindowControlPanel::AddButtonClicked);
    connect(ui_->remove_button, &QPushButton::clicked, this, &MainWindowControlPanel::RemoveButtonClicked);

    UpdateMessage(Message::kEmpty);
}

void MainWindowControlPanel::AddButtonClicked() {
    auto [key, ok] = ReadNodeKey();
    if (ok) {
        emit AddNode(key);
        return;
    }
    UpdateMessage(Message::kInvalidKeyEntered);
}

void MainWindowControlPanel::RemoveButtonClicked() {
    auto [key, ok] = ReadNodeKey();
    if (ok) {
        emit RemoveNode(key);
        return;
    }
    UpdateMessage(Message::kInvalidKeyEntered);
}

void MainWindowControlPanel::UpdateMessage(Message mes) {
    const char* text = nullptr;
    switch (mes) {
        case Message::kEmpty:
            text = "";
            break;
        case Message::kUnknownErrorOccurred:
            text = "Unknown error occurred";
            break;
        case Message::kKeyExists:
            text = "Node key already exists";
            break;
        case Message::kKeyNotExists:
            text = "Node key is not exists";
            break;
        case Message::kInvalidKeyEntered:
            text = "Invalid node key value (should be positive integer)";
            break;
        case Message::kNodeAdded:
            text = "Node added";
            break;
        case Message::kNodeRemoved:
            text = "Node removed";
            break;
    }
    ui_->message_label->setText(text);
}

std::pair<unsigned int, bool> MainWindowControlPanel::ReadNodeKey() {
    bool ok = false;
    auto node_key = ui_->node_key_lineedit->text().toInt(&ok);
    ui_->node_key_lineedit->clear();
    if (ok && node_key >= 1) {
        return { node_key, true };
    }
    return { 0, false };
}

MainWindowControlPanel::~MainWindowControlPanel() noexcept {

}

