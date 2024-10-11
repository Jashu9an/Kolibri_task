#include "filemodifier.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    FileModifier modifier;
    modifier.setFileMask("*.txt");  // Маска файлов
    modifier.setDeleteInput(false);  // Не удалять исходные файлы
    modifier.setSavePath("C:/output");  // Путь для сохранения
    modifier.setOverwriteBehavior(false);  // Не перезаписывать, добавлять счетчик к имени
    modifier.setTimerEnabled(true);  // Включить таймер
    modifier.setTimerInterval(5000);  // Периодичность проверки 5 секунд
    modifier.setXORKey(QByteArray::fromHex("AABBCCDDEEFF0011"));  // 8-байтное значение для XOR

    modifier.startProcessing();

    return a.exec();
}
