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
#include <fstream>
void Graph::build() {
	delete m_graphPath;
	m_graphPath = new QPainterPath;

	int minX = getText(m_win -> m_limitMin -> getInputField()).toInt();
	int maxX = getText(m_win -> m_limitMax -> getInputField()).toInt();
	double dx = double(maxX - minX) / this -> width();

	QList<std::optional<double>> points;
	points.reserve(this -> width() + 1);
	for (int i = 0; i < this -> width() + 1; ++i)
		points.append(std::nullopt);

	m_init = false;
	double minY = std::numeric_limits<double>::max();
	double maxY = -std::numeric_limits<double>::max();
	size_t gx = 0;
	auto tokens = Calc::postfixList(getText(m_win -> m_function -> getInputField()));
	CalcX calc;

	std::ofstream O;
	O.open("C:/Users/User/Desktop/debug.txt");
	O << "-------------------- Preinit -------------------------\n";
	for (double x = minX; x < maxX; x += dx) {
		if (abs(x) < __FLT_EPSILON__) // Fix FP precision
			x = 0;
		points[gx] = calc(tokens, x);
		if (points[gx]) {
			minY = std::min(points[gx].value(), minY);
			maxY = std::max(points[gx].value(), maxY);
			O << gx << " | x: " << x << ", y: " << points[gx].value() << ", minY: " << minY << ", maxY: " << maxY << '\n';
		}
		++gx;
	}

	if (abs(maxY - minY) < __FLT_EPSILON__) {
		minY -= 5;
		maxY += 5;
	}
	double k = double(this -> height()) / (maxY - minY);
	gx = 0;
	O << "minY: " << minY << ", maxY: " << maxY << ", h: " << this -> height() << ", k: " << k << '\n';
	for (auto y : points) {
		if (y) {
			O << "gx: " << gx << ", gy: " << k * (maxY - y.value()) << '\n';
			if (m_init) { // y has a value and this is not the first point
				m_graphPath -> lineTo(gx, k * (maxY - y.value()));
			} else { // y has a value and this is the first point
				m_init = true;
				m_graphPath -> moveTo(gx, k * (maxY - y.value()));
			}
		} else { // y is std::nullopt
			m_graphPath -> moveTo(gx, -1);
		}
		++gx;
	}

	O << "bwaaa\n";
	for (size_t i = 0; i < 10; ++i)
		O << m_graphPath -> elementAt(i).x << " " << m_graphPath -> elementAt(i).y << '\n';

	m_ogPathRect = m_graphPath -> boundingRect();

	this -> update();

	return;
}

void Graph::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(m_win -> SettingsValues.graphLineColor);
	pen.setWidth(4);
	painter.setPen(pen);

	m_graphPath -> // add transform pls also try `1/(x-5)` cuz it shows const idk
	painter.drawPath(*m_graphPath);

	return;
}

Graph::~Graph() {
	delete m_graphPath;
}