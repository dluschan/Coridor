#include "sprite.h"

Sprite::Sprite(QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
{
    currentFrame = 0; // Устанавливаем координату текущего кадра спрайта
    spriteImage = new QPixmap(":PICTURES/f3_nimbus.png"); // Загружаем изображение спрайта в QPixmap
    // spriteImage->setDevicePixelRatio(0.5);
    dvig = true;
    x = 0;
    y = 0;
    t = 101;
    u = 101;
    time = 7.5;

    timer = new QTimer(); // Создаём таймер для анимации спрайта
    timer2 = new QTimer();
    // Подключаем сигнал от таймера к слоту перелистывания кадров спрайта
    connect(timer, &QTimer::timeout, this, &Sprite::nextFrame);
    connect(timer2, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(75); // Запускаем спрайт на генерацию сигнала с периодичность 25 мс
    timer2->start(0.001);
}

QRectF Sprite::boundingRect() const
{
    //(-190, -140, 380, 280) (-2,-2,+4,+4)
    return QRectF(0, 0, 101, 101);
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
    painter->drawPixmap(0, 0, *spriteImage, 0, currentFrame, 101, 101);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Sprite::nextFrame()
{
    /* По сигналу от таймера передвигаем на 20 пикселей точку отрисовки
     * Если currentFrame = 300 то обнуляем его, поскольку размер sprite sheet 300 пикселей на 20
     * */
    currentFrame += 101;
    if (currentFrame >= 808)
        currentFrame = 0;
    this->update(); // и перерисовываем графический объект с новым кадром спрайта
}

void Sprite::move()
{
    if (dvig)
    {
        distance = sqrt((t - x) * (t - x) + (u - y) * (u - y)) * 64;
        x += 0.2 * time * (t - x) / distance;
        y += 0.2 * time * (u - y) / distance;
    }
    // sprite.setPosition(550 + x * 64 + 31 - w * 0.75, 275 + y * 64 + 31 - h);
    setPos(x, y);
    if (abs(t - x) < 0.1 && abs(u - y) < 0.1)
    {
        x = t;
        y = u;
        // if (move) { flag = true; act = false; }
        dvig = false;
        // sprite.setPosition(550 + x * 64 + 31 - w * 0.75, 275 + y * 64 + 31 - h);
        setPos(x, y);
    }
}
