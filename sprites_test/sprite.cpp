#include "sprite.h"

Sprite::Sprite(QObject* parent)
    : QObject(parent)
    , QGraphicsItem()
{
    currentFrame = 0; // Устанавливаем координату текущего кадра спрайта
    spriteImage = new QPixmap(":PICTURES/f3_nimbus.png"); // Загружаем изображение спрайта в QPixmap
    spriteImage->setDevicePixelRatio(1);

    timer = new QTimer(); // Создаём таймер для анимации спрайта
    // Подключаем сигнал от таймера к слоту перелистывания кадров спрайта
    connect(timer, &QTimer::timeout, this, &Sprite::nextFrame);
    timer->start(200); // Запускаем спрайт на генерацию сигнала с периодичность 25 мс
}

QRectF Sprite::boundingRect() const
{
    return QRectF(-188, -138, 376, 276); //(-190, -140, 380, 280) (-2,-2,+4,+4)
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
    painter->drawPixmap(89, 39, *spriteImage, 0, currentFrame, 101, 101);
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
