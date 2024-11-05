#include "mainwindow.h"
#include "voleMachine.h"
#include <QApplication>
#include <QFileDialog>
#include <QPushButton>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    Machine machine;

    QPushButton loadButton("Load File", &w);
    loadButton.setGeometry(QRect(QPoint(100, 100), QSize(200, 50)));

    QPushButton runButton("Run", &w);
    runButton.setGeometry(QRect(QPoint(100, 160), QSize(200, 50)));

    QPushButton printStateButton("Print State", &w);
    printStateButton.setGeometry(QRect(QPoint(100, 220), QSize(200, 50)));

    std::string fileNameStd;

    QObject::connect(&loadButton, &QPushButton::clicked, [&]() {
        QString fileName = QFileDialog::getOpenFileName(nullptr, "Open File", "", "Files (*.*)");

        if (!fileName.isEmpty()) {
            fileNameStd = fileName.toStdString();

            try {
                machine.loadProgramFile(fileNameStd);
            } catch (const std::bad_alloc& e) {
                std::cerr << "Memory allocation failed while loading program: " << e.what() << std::endl;
            }
        }
    });

    QObject::connect(&runButton, &QPushButton::clicked, [&]() {
        try {
            machine.run(); // Call run when the button is clicked
        } catch (const std::exception& e) {
            std::cerr << "Error while running the program: " << e.what() << std::endl;
        }
    });

    QObject::connect(&printStateButton, &QPushButton::clicked, [&]() {
        machine.outputState(); // Call outputState when the button is clicked
    });

    w.show();

    return a.exec();
}
