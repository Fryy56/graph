#pragma once

#include "tintinglabel.hpp"
#include <QLayout>
#include <QLineEdit>
#include <QEvent>


class HighlightedLineEdit : public QWidget {
	Q_OBJECT

	Q_PROPERTY(QColor borderColor READ propGetBorderColor WRITE propSetBorderColor)

public:
	explicit HighlightedLineEdit(QWidget* parent = nullptr);
	explicit HighlightedLineEdit(QString const& labelText, QWidget* parent = nullptr);
	explicit HighlightedLineEdit(QString const& labelText, QString const& placeholderText, QWidget* parent= nullptr);
	~HighlightedLineEdit() override;

	static void setColors(
		QColor const& offLabelColor = Colors.offLabelColor,
		QColor const& onLabelColor = Colors.onLabelColor,
		QColor const& fieldsColor = Colors.fieldsColor,
		QColor const& textColor = Colors.textColor,
		QColor const& borderColor = Colors.borderColor
	);

	// Getters (and technically setters)
	QVBoxLayout* getMainLayout() const { return m_mainLayout; }
	QHBoxLayout* getLabelLayout() const { return m_labelLayout; }
	TintingLabel* getLabel() const { return m_label; }
	QLineEdit* getInputField() const { return m_inputField; }
	void heightViaLabel(int height);
	void widthViaField(int width);
	void setLabelStyleSheet(QString const& text) { m_label -> setStyleSheet(text); }
	void setFieldStyleSheetSafe(QString const& styleSheet); //! USE THIS FOR ALL CHANGES
	void setFieldToolTip(QString const& text) { m_inputField -> setToolTip(text); };
	void setFieldMaxLength(int length) { m_inputField -> setMaxLength(length); };
	void setFieldValidator(QValidator const* val) { m_inputField -> setValidator(val); }
	void setFieldText(QString const& text) { m_inputField -> setText(text); }

protected:
	bool eventFilter(QObject*, QEvent*) override;

	// Property getters/setters
	QColor propGetBorderColor() const { return m_curBorderColor; };
	void propSetBorderColor(QColor const&);

	// Fields
	QColor m_curBorderColor;
	static QString const m_defaultFieldStyleSheetTemplate;
	QString m_defaultFieldStyleSheet;
	QString m_customFieldStyleSheet;

	inline static struct {
		QColor offLabelColor;
		QColor onLabelColor;
		QColor fieldsColor;
		QColor textColor;
		QColor borderColor;
	} Colors;
public: // Funny getter
	static decltype(Colors) getColors() { return Colors; }
protected:

	QVBoxLayout* m_mainLayout;

	// Objecttree (m_mainLayout)
		QHBoxLayout* m_labelLayout;
			TintingLabel* m_label;
		QLineEdit* m_inputField;
	// Objecttree (m_mainLayout)

	QPropertyAnimation* m_clearPulse;

public slots:
	void clearWithPulse();
	void pulse();
};