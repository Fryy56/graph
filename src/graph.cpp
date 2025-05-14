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
	m_path = new QPainterPath;
}
#include <fstream>
void Graph::build() {
	m_path -> clear();
	Calc calc;

	auto expr = getText(m_win -> m_function -> getInputField());
	int minX = getText(m_win -> m_limitMin -> getInputField()).toInt();
	int maxX = getText(m_win -> m_limitMax -> getInputField()).toInt();
	double dx = double(maxX - minX) / this -> width();

	QList<std::optional<double>> points(this -> width() + 1);

	/* points[0] = calc(QString(expr).replace('x', QString::number(minX)));
	if (points[0]) {
		m_path -> moveTo(0, points[0].value());
		m_init = true;
	} else
		m_init = false; */

	double minY = std::numeric_limits<double>::max();
	double maxY = std::numeric_limits<double>::min();
	size_t gx = 0;
	std::ofstream O;
	O.open("C:/Users/User/Desktop/debug.txt");
	for (double x = minX; x < maxX; x += dx) {
		if (abs(x) < __FLT_EPSILON__) // Fix FP precision
			x = 0;
		points[gx] = calc(QString(expr).replace('x', QString::number(x)));
		if (points[gx]) {
			minY = std::min(points[gx].value(), minY);
			maxY = std::max(points[gx].value(), maxY);
			O << "y: " << points[gx].value() << ", minY: " << minY << ", maxY: " << maxY << '\n';
		}
		++gx;
	}

	double k = double(this -> height()) / (maxY - minY);
	gx = 0;
	O << "minY: " << minY << ", maxY: " << maxY << ", h: " << this -> height() << ", k: " << k << '\n';
	for (auto y : points) {
		if (y.value()) {
			O << "gx: " << gx << ", gy: " << k * (maxY - y.value()) << '\n';
			if (m_init) { // y has a value and this is not the first point
				m_path -> lineTo(gx, k * (maxY - y.value()));
			} else { // y has a value and this is the first point
				m_init = true;
				m_path -> moveTo(gx, k * (maxX - y.value()));
			}
		} else { // y is std::nullopt
			m_path -> moveTo(gx, -1);
		}
		++gx;
	}

	this -> update();

	return;
}

void Graph::paintEvent(QPaintEvent*) {
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	QPen pen(m_win -> SettingsValues.graphLineColor);
	pen.setWidth(4);
	painter.setPen(pen);
	painter.drawPath(*m_path);

	return;
}

Graph::~Graph() {
	delete m_path;
}