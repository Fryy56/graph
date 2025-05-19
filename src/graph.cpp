#include "graph.hpp"
#include "calc.hpp"
#include <limits>
#include <QPainter>


static QString getText(QLineEdit* inputField) {
	auto ret = inputField -> text();
	if (ret.isEmpty())
		return inputField -> placeholderText();
	else
		return ret;
}

Graph::Graph(MainWindow* win, QWidget* parent) : QWidget(parent), m_win(win) {
	m_graphPath = new QPainterPath;
}

void Graph::updateWidth(int newWidth) {
	this -> setFixedWidth(newWidth);

	return;
}
#include <fstream>
void Graph::build() {
	m_graphPath -> clear();

	int minX = getText(m_win -> m_limitMin -> getInputField()).toInt();
	int maxX = getText(m_win -> m_limitMax -> getInputField()).toInt();
	double dx = double(maxX - minX) / this -> width();

	QList<std::optional<double>> points;
	points.reserve(this -> width() + 1);
	for (int i = 0; i < this -> width() + 1; ++i)
		points.append(std::nullopt);

	m_init = false;
	size_t gx = 0;
	auto tokens = Calc::postfixList(getText(m_win -> m_function -> getInputField()));
	CalcX calc;

	std::ofstream O;
	O.open("C:/Users/User/Desktop/debug.txt");
	O << "-------------------- Preinit -------------------------\n";
	O << "Tokens:\n";
	for (auto i : tokens)
		O << i.toStdString() << '\n';
	O << "------------------------------------------------------\n";
	for (double x = minX; x < maxX; x += dx) {
		if (abs(x) < __FLT_EPSILON__) // Fix FP precision
			x = 0;
		points[gx] = calc(tokens, x);
		if (points[gx]) {
			O << gx << " | x: " << x << ", y: " << points[gx].value() << '\n';
		}
		++gx;
	}

	gx = 0;
	int y0 = this -> height() / 2;
	for (auto y : points) {
		if (y) {
			O << "gx: " << gx << ", gy: " << y.value() << '\n';
			if (m_init) { // y has a value and this is not the first point
				m_graphPath -> lineTo(gx, y0 - y.value());
			} else { // y has a value and this is the first point
				m_init = true;
				m_graphPath -> moveTo(gx, y0 - y.value());
			}
		} else { // y is std::nullopt
			m_graphPath -> moveTo(gx, -1);
		}
		++gx;
	}

	O << "bwaaa\n";
	for (size_t i = 0; i < 10; ++i)
		O << m_graphPath -> elementAt(i).x << " " << m_graphPath -> elementAt(i).y << '\n';

	this -> update();

	return;
}

void Graph::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(m_win -> SettingsValues.graphLineColor);
	pen.setWidth(4);
	painter.setPen(pen);

	painter.drawPath(*m_graphPath);

	return;
}

Graph::~Graph() {
	delete m_graphPath;
}

// ---------------------------------------------------------------------------

GraphView::GraphView(QGraphicsScene* scene, QWidget* parent) : QGraphicsView(scene, parent) {}

void GraphView::resizeEvent(QResizeEvent*) {
	emit widthUpdated(this -> width());

	return;
}