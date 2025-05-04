#pragma once

#include "tintinglabel.hpp"
#include <QLayout>
#include <QLineEdit>
#include <QEvent>


class HighlightedLineEdit final : public QWidget {
	Q_OBJECT

public:
	explicit HighlightedLineEdit(QWidget* parent = nullptr);
	explicit HighlightedLineEdit(QString const& labelText, QWidget* parent = nullptr);
	explicit HighlightedLineEdit(QString const& labelText, QString const& placeholderText, QWidget* parent= nullptr);
	~HighlightedLineEdit() override;

	static void setColors(
		QColor const&& offLabelColor = std::move(Colors.offLabelColor),
		QColor const&& onLabelColor = std::move(Colors.onLabelColor),
		QColor const&& fieldsColor = std::move(Colors.fieldsColor),
		QColor const&& textColor = std::move(Colors.textColor)
	);

	// Getters (and technically setters)
	QHBoxLayout& getLayout() { return *m_labelLayout; }
	TintingLabel& getLabel() { return *m_label; }
	QLineEdit& getInputField() { return *m_inputField; }
	void heightViaLabel(int height);
	void widthViaField(int width);
	void setLabelStyleSheet(QString const& text) { m_label -> setStyleSheet(text); }
	void setFieldToolTip(QString const& text) { m_inputField -> setToolTip(text); };
	void setFieldMaxLength(int length) { m_inputField -> setMaxLength(length); };
	void setFieldValidator(QValidator const* val) { m_inputField -> setValidator(val); }

private:
	bool eventFilter(QObject*, QEvent*) override;

	// Fields
	inline static struct {
		QColor offLabelColor;
		QColor onLabelColor;
		QColor fieldsColor;
		QColor textColor;
	} Colors;

	QVBoxLayout* m_mainLayout;
		QHBoxLayout* m_labelLayout;
			TintingLabel* m_label;
		QLineEdit* m_inputField;
};