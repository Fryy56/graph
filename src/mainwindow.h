#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
	class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
	Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = nullptr);
	~MainWindow();

private:
	QWidget* m_centralWidget;

	QHBoxLayout* m_mainLayout;
	QVBoxLayout* m_graphLayout;
	QVBoxLayout* m_buttonLayout;

	QWidget* m_graph;
	QLineEdit* m_function;
private slots:
};