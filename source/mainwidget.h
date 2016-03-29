#ifndef WIDGET_H
#define WIDGET_H

#include <QClipboard>
#include <QColorDialog>
#include <QFileDialog>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMenu>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QScreen>
#include <QVBoxLayout>

#include "settings.h"

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = 0);
    ~MainWidget();

    void setLenseVisible(bool visible);

private:
    // The struct for markers list. Keeps a pen of a marker and a path.
    struct MarkerPath
    {
        QPainterPath path;
        QPen pen;
    };

    QMenu *m_contextMenu;

    QPoint m_lastMousePos = QPoint();
    QRect m_selectionRect = QRect();
    QList<MainWidget::MarkerPath> m_markerPaths;
    QPainterPath m_markerPath;

    QPixmap m_buffer; //pixmap for screenshot image

    bool m_lenseIsVisible = false;
    bool m_markerIsVisible = false;

    QPen m_framePen = QPen(Qt::blue, 1, Qt::DashLine); //The pen of a selection frame
    QPen m_markerPen = QPen(Qt::yellow, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin); //The pen of a current marker style

    int m_currentScaleFactor = 4; //The lense scale factor

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    void paintEvent(QPaintEvent *e) override;
    void showEvent(QShowEvent *) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

    void drawMarkerPaths(QPainter &p); //Draw markers with painter p

public slots:
    void save();
    void saveByName(const QString &fileName);
    void saveAs();
    void copy();
};

#endif // WIDGET_H
