#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent)
{
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool); //A fullscreen window without a icon on task dashboard.
    setAttribute(Qt::WA_QuitOnClose, false); //Don't close the app after closing this widget.

    m_contextMenu = new QMenu(this);

    QAction *saveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::Key_Enter));
    QAction *alternateSaveAction = new QAction(tr("Save"), this);
    saveAction->setShortcut(QKeySequence(Qt::Key_Return));

    QAction *copyAction = new QAction(tr("Copy to clipboard"), this);
    copyAction->setShortcut(QKeySequence(QKeySequence::Copy));

    QAction *selectAction = new QAction(tr("Select"), this);
    QAction *redMarkerAction = new QAction(tr("Red marker"), this);
    QAction *yellowMarkerAction = new QAction(tr("Yellow marker"), this);
    QAction *blueMarkerAction = new QAction(tr("Blue marker"), this);
    QAction *blackMarkerAction = new QAction(tr("Black marker"), this);
    QAction *whiteMarkerAction = new QAction(tr("White marker"), this);
    QAction *customMarkerAction = new QAction(tr("Custom marker"), this);

    QAction *saveAsAction = new QAction(tr("Save As.."), this);
    saveAsAction->setShortcut(QKeySequence(QKeySequence::Save));

    QAction *cancelAction = new QAction(tr("Cancel"), this);
    cancelAction->setShortcut(QKeySequence(Qt::Key_Escape));

    QAction *undoAction = new QAction(tr("Undo"), this);
    undoAction->setShortcut(QKeySequence(QKeySequence::Undo));

    //connections
    connect(saveAction, &QAction::triggered, this, &MainWidget::save);
    connect(alternateSaveAction, &QAction::triggered, this, &MainWidget::save);

    connect(copyAction, &QAction::triggered, this, &MainWidget::copy);

    connect(selectAction, &QAction::triggered, [&]()
    {
        m_markerIsVisible = false;
    });

    connect(redMarkerAction, &QAction::triggered, [&]()
    {
        m_markerPen.setColor(Qt::red);
        m_markerIsVisible = true;
    });

    connect(yellowMarkerAction, &QAction::triggered, [&]()
    {
        m_markerPen.setColor(Qt::yellow);
        m_markerIsVisible = true;
    });

    connect(blueMarkerAction, &QAction::triggered, [&]()
    {
        m_markerPen.setColor(Qt::blue);
        m_markerIsVisible = true;
    });

    connect(blackMarkerAction, &QAction::triggered, [&]()
    {
        m_markerPen.setColor(Qt::black);
        m_markerIsVisible = true;
    });

    connect(whiteMarkerAction, &QAction::triggered, [&]()
    {
        m_markerPen.setColor(Qt::white);
        m_markerIsVisible = true;
    });

    connect(customMarkerAction, &QAction::triggered, [&]()
    {
        QColor markerColor = QColorDialog::getColor(m_markerPen.color(), this, tr("Color"), QColorDialog::ShowAlphaChannel);
        if(!markerColor.isValid())
            return;
        m_markerPen.setColor(markerColor);
        m_markerIsVisible = true;
    });

    connect(undoAction, &QAction::triggered, [&]()
    {
        if(m_markerPaths.isEmpty())
            return;

        m_markerPaths.removeLast();
    });

    connect(saveAsAction, &QAction::triggered, this, &MainWidget::saveAs);

    connect(cancelAction, &QAction::triggered, this, &MainWidget::close);

    m_contextMenu->addAction(saveAction);
    m_contextMenu->addAction(copyAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(selectAction);
    m_contextMenu->addAction(redMarkerAction);
    m_contextMenu->addAction(yellowMarkerAction);
    m_contextMenu->addAction(blueMarkerAction);
    m_contextMenu->addAction(blackMarkerAction);
    m_contextMenu->addAction(whiteMarkerAction);
    m_contextMenu->addAction(customMarkerAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(saveAsAction);
    m_contextMenu->addSeparator();
    m_contextMenu->addAction(cancelAction);

    addAction(saveAction);
    addAction(cancelAction);
    addAction(copyAction);
    addAction(alternateSaveAction);
    addAction(selectAction);
    addAction(redMarkerAction);
    addAction(yellowMarkerAction);
    addAction(blueMarkerAction);
    addAction(blackMarkerAction);
    addAction(whiteMarkerAction);
    addAction(customMarkerAction);
    addAction(saveAsAction);
    addAction(cancelAction);
    addAction(undoAction);
}


MainWidget::~MainWidget()
{
}

void MainWidget::setLenseVisible(bool visible)
{
    if(visible)
        setCursor(Qt::BlankCursor);
    else
        setCursor(Qt::CrossCursor);

    m_lenseIsVisible = visible;
}

void MainWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == SC_LENSE_MODIFER)
    {
        setLenseVisible(event->modifiers() == SC_LENSE_MODIFER); //Enable lense
        update();
    }
}

void MainWidget::keyReleaseEvent(QKeyEvent *event)
{
    setLenseVisible(event->modifiers() == SC_LENSE_MODIFER); //Disable lense
    update();
}

void MainWidget::mousePressEvent(QMouseEvent *event)
{
    m_lastMousePos = event->pos();
    if(m_markerIsVisible && event->button() == Qt::LeftButton)
    {
        m_markerPath.moveTo(event->pos()); //An initial point of a current marker
    }
    else if(event->button() == Qt::MiddleButton)
        save();
}

void MainWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(Settings::settings().saveAfterSelection)
        save();
    else if(Settings::settings().copyAfterSelection)
        copy();

    if(m_markerIsVisible && event->button() == Qt::LeftButton)
    {
        //Add a current marker in the list
        MainWidget::MarkerPath markerPath;
        markerPath.path = m_markerPath;
        markerPath.pen = m_markerPen;
        m_markerPaths.append(markerPath);

        m_markerPath = QPainterPath();
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::LeftButton)
    {
        if(m_markerIsVisible)
        {
            m_markerPath.lineTo(event->pos()); //Draw marker
        }
        else
        {
            //Install selection
            m_selectionRect.setTopLeft(m_lastMousePos);
            m_selectionRect.setBottomRight(event->pos());
        }
    }
    update();
}

void MainWidget::wheelEvent(QWheelEvent *event)
{
    if(m_lenseIsVisible)
    {
        //Rescale lense
        event->delta() > 0 ? m_currentScaleFactor++ : m_currentScaleFactor--;
        if(m_currentScaleFactor > 10) m_currentScaleFactor = 10;
        if(m_currentScaleFactor <  1) m_currentScaleFactor =  1;
        update();
    }
    else if(m_markerIsVisible)
    {
        //Change a pen tidth of a current marker
        event->delta() > 0 ? m_markerPen.setWidth(m_markerPen.width() + 1) : m_markerPen.setWidth(m_markerPen.width() - 1);
        if(m_markerPen.width() > 50) m_markerPen.setWidth(50);
        if(m_markerPen.width() <  1) m_markerPen.setWidth( 1);
        update();
    }
}

void MainWidget::paintEvent(QPaintEvent *e)
{
    QPixmap screen(m_buffer); //A temporary pixmap. We write all drawing operation in in this pixmap. With this we keep the original pixmap.

    QPainter p;
    p.begin(&screen);

    //p.drawPixmap(rect(), m_buffer);

    drawMarkerPaths(p);

    if(m_markerIsVisible)
    {
        p.setPen(m_markerPen);
        p.drawPoint(QCursor::pos());

        if(!m_markerPath.isEmpty())
            p.drawPath(m_markerPath);
    }

    if(!m_selectionRect.isNull())
    {
        p.setPen(m_framePen);
        p.drawRect(m_selectionRect.adjusted(0, 0, -1, -1));
    }

    if(Settings::settings().fillBackgroind)
    {
        QPainterPath path;
        path.addRect(rect());
        path.addRect(m_selectionRect);
        p.fillPath(path, QBrush(Settings::settings().fillColor, Qt::SolidPattern));
    }

    p.end();

    //draw the lense
    if(m_lenseIsVisible)
    {
        p.begin(&screen) ;
        p.drawLine(QCursor::pos().x() - 2, QCursor::pos().y()    , QCursor::pos().x() + 2, QCursor::pos().y()    );
        p.drawLine(QCursor::pos().x()    , QCursor::pos().y() - 2, QCursor::pos().x()    , QCursor::pos().y() + 2);


        int lenseSize = Settings::settings().lenseSize;

        QRect bufferRect = QRect(0, 0, lenseSize / m_currentScaleFactor, lenseSize / m_currentScaleFactor);
        bufferRect.moveCenter(QCursor::pos());
        bufferRect &= screen.rect();

        QPixmap lense = screen.copy(bufferRect);

        QTransform transform;
        transform.translate(QCursor::pos().x(), QCursor::pos().y());
        transform.scale(m_currentScaleFactor, m_currentScaleFactor);

        QRect r(bufferRect);
        r.moveCenter(QPoint(0, 0));
        QRect lenseRect = transform.mapRect(r).normalized();

        if(lenseRect.left() < rect().left()) lenseRect.moveLeft(rect().left());
        if(lenseRect.right() > rect().right()) lenseRect.moveRight(rect().right());
        if(lenseRect.top() < rect().top()) lenseRect.moveTop(rect().top());
        if(lenseRect.bottom() > rect().bottom()) lenseRect.moveBottom(rect().bottom());

        p.drawPixmap(lenseRect, lense);

        p.setPen(QPen(Qt::blue, 2));
        p.drawRect(lenseRect);
        p.drawRect(bufferRect);
        p.end();
    }

    //Now we draw the "screen" pixmap on the widget.
    p.begin(this);
    p.drawPixmap(rect(), screen);
    p.end();

    QWidget::paintEvent(e);
}

void MainWidget::showEvent(QShowEvent *)
{
    //Initializing new image and all tools.
    m_currentScaleFactor = Settings::settings().lenseScaleFactor;

    m_buffer = QGuiApplication::primaryScreen()->grabWindow(0);
    m_markerPaths.clear();
    m_markerIsVisible = false;

    if(!Settings::settings().saveSelectionFrame)
        m_selectionRect = QRect();

    setGeometry(QGuiApplication::primaryScreen()->geometry());
    activateWindow();
}

void MainWidget::contextMenuEvent(QContextMenuEvent *event)
{
    setLenseVisible(false);
    m_contextMenu->exec(event->globalPos());
}

void MainWidget::drawMarkerPaths(QPainter &p)
{
    if(!m_markerPaths.isEmpty())
        foreach (MainWidget::MarkerPath markerPath, m_markerPaths) {
            p.setPen(markerPath.pen);
            p.drawPath(markerPath.path);
    }
}

void MainWidget::save()
{
    saveByName(Settings::absoluteFileName());
}

void MainWidget::saveByName(const QString &fileName)
{
    QPixmap screenShot;

    QPainter p;
    //Draw markers on the original pixmap.
    p.begin(&m_buffer);
    drawMarkerPaths(p);
    p.end();

    if(m_selectionRect.isNull())
        screenShot = m_buffer;
    else
        screenShot = m_buffer.copy(m_selectionRect);

    if(!screenShot.save(fileName, Settings::settings().imageFormat.toLocal8Bit().constData()))
        QMessageBox::warning(nullptr, "QScreenShotTool:", tr("Cannot save to file: ") + Settings::absoluteFileName());

    if(Settings::settings().closeAfterSave)
        close();
}

void MainWidget::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as.."), Settings::absoluteFileName());
    if(fileName.isEmpty())
        return;

    saveByName(fileName);
}

void MainWidget::copy()
{
    QPixmap screenShot;

    QPainter p;
    //Draw markers on the original pixmap.
    p.begin(&m_buffer);
    drawMarkerPaths(p);
    p.end();

    if(m_selectionRect.isNull())
        screenShot = m_buffer;
    else
        screenShot = m_buffer.copy(m_selectionRect);

    QGuiApplication::clipboard()->setPixmap(screenShot);

    if(Settings::settings().closeAfterCopy)
        close();
}

