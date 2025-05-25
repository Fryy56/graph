#include <QApplication>
#include <QSplashScreen>
#include <qsystemdetection.h>
#include "mainwindow.hpp"
#include <QMessageBox>
#include "styles.hpp"


int main(int argc, char* argv[]) {
	QApplication a(argc, argv);
	QSplashScreen loadingScreen(QPixmap(":/icon.png").scaled(200, 200));
	loadingScreen.show();
	MainWindow w;

	// Styles setup
	a.setStyleSheet(style_QToolTip);
	a.setFont(QFont("Lucida Sans Unicode", 10));

	#ifndef Q_OS_WINDOWS
		w.setWindowIcon(QIcon(":/icon.png"));
	#endif

	try {
		loadingScreen.finish(&w);
		w.show();
		return a.exec();
	} catch(...) {
		std::exception_ptr except = std::current_exception();
		try {
			std::rethrow_exception(except);
		} catch (std::exception const& exceptRet) {
			QMessageBox::critical(
				nullptr,
				"Error",
				QString("An exception has occurred:\n%1").arg(exceptRet.what())
			);
			return 1;
		} catch (int except) {
			QMessageBox::critical(
				nullptr,
				"Error",
				QString(QString("Debug throw: %1").arg(except))
			);
			return 1;
		} catch (...) {
			QMessageBox::critical(
				nullptr,
				"Error",
				QString("An unhandled exception has occurred.")
			);
			return 1;
		}
	}
}