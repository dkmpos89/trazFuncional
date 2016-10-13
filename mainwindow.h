#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "analizador.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void buscarEnExcel(QString prc, QString fileExcelPlanilla);
    void buscarFuncionalidad(QString fileExcelFuncionalidad);
public slots:
    void imprimirSalida(QStringList lista);
private slots:
    void on_btnSetear_clicked();

    void on_toolPlanilla_clicked();

    void on_toolFuncionalidad_clicked();

    void on_actionEliminar_Duplicados_triggered();

    void on_actionEliminar_configuraciones_triggered();

    void on_btnEditar_clicked();

private:
    Ui::MainWindow *ui;
    QStringList relaciones;
    analizador* calc;
};

#endif // MAINWINDOW_H
