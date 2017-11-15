#include "sprite.h"

Sprite::Sprite(QString PN, qreal X, qreal Y, int S, int FN, QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
    , spriteImage(new QPixmap(":PICTURES/" + PN + ".png"))
    , x(X)
    , y(Y)
    , size(S)
    , framesNumber(FN)
    , time(7.5)
    , timer(new QTimer)
    , timer2(new QTimer)
{
    setFlag(ItemIsSelectable);

    connect(timer, SIGNAL(timeout()), this, SLOT(nextFrame()));
    connect(timer2, SIGNAL(timeout()), this, SLOT(move()));
    connect(this, SIGNAL(released(qreal, qreal)), this, SLOT(select(qreal, qreal)));
    timer->start(75);
    timer2->start(18);
}

QRectF Sprite::boundingRect() const
{
    return QRectF(x, y, size, size);
}

void Sprite::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (!dvig)
        selected = true;
    update();
    QGraphicsItem::mousePressEvent(event);
}

void Sprite::mouseReleaseEvent(QGraphicsSceneMouseEvent* event)
{
    if (selected)
    {
        dvig = true;
        emit released(mx, my);
    }
    selected = false;
    update();
    QGraphicsItem::mouseReleaseEvent(event);
}

void Sprite::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
    /* В отрисовщике графического объекта отрисовываем спрайт
     * Разберём все параметры данной функции
     * Первых два аргумента - это координат X и Y куда помещается QPixmap
     * Третий аргумент - это указатель на QPixmap
     * 4 и 5 аргументы - Координаты в В изображении QPixmap, откуда будет отображаться изображение
     * Задавая координату X с помощью перемнной currentFrame мы будем как бы передвигать камеру
     * по спрайту
     * и последние два аргумента - это ширина и высота отображаем части изображение, то есть кадра
     * */
    //(89, 39, *spriteImage, 0, currentFrame, 101, 101)
    painter->drawPixmap(x, y, *spriteImage, 0, currentFrame, size, size);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Sprite::nextFrame()
{
    /* По сигналу от таймера передвигаем на 20 пикселей точку отрисовки
     * Если currentFrame = 300 то обнуляем его, поскольку размер sprite sheet 300 пикселей на 20
     * */
    currentFrame += size;
    if (currentFrame >= size * framesNumber)
        currentFrame = 0;
    setPos(x, y);
    this->update(); // и перерисовываем графический объект с новым кадром спрайта
}

void Sprite::move()
{
    if (dvig)
    {
        qreal distance = sqrt((t - x) * (t - x) + (u - y) * (u - y));
        x += 0.2 * time * (t - x) / distance;
        y += 0.2 * time * (u - y) / distance;
    }
    setPos(x, y);
    if (abs(t - x) < 0.1 && abs(u - y) < 0.1)
    {
        x = t;
        y = u;
        dvig = false;
        setPos(x, y);
    }
}

void Sprite::select(qreal i, qreal j)
{
    qDebug() << "HERE";
    t = i;
    u = j;
}

void Sprite::updateCurPos(qreal x, qreal y)
{
    mx = x;
    my = y;
}
