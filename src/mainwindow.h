#pragma once

#include <QMainWindow>
#include <QPushButton>
#include <QLayout>
#include <QLineEdit>
#include <QLabel>

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
	struct {
		QColor BG = QColor(25, 25, 25);
		QColor Fields = QColor(45, 45, 45);
	} Colors;

	QWidget* m_centralWidget;

	QHBoxLayout* m_mainLayout;
	QVBoxLayout* m_graphLayout;
	QVBoxLayout* m_functionLayout;
	QHBoxLayout* m_functionLabelLayout;
	QVBoxLayout* m_buttonLayout;

	QWidget* m_graph;
	QLabel* m_functionLabel;
	QLineEdit* m_functionField;

private slots:
	void onFuncFieldSelect();
	void onFuncFieldDeselect();
};