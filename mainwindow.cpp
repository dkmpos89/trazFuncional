#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "xlsxdocument.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QThread>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->btnGO->setEnabled(false);
    ui->btnEditar->setVisible(false);

    QString path = QDir::currentPath()+"/config";

    if(!QDir(path).exists())
        QDir().mkdir(QDir::currentPath()+"/config");

    QFile file(path+"/pathPlanilla.txt");
    file.open(QIODevice::ReadWrite);
    QTextStream streamMO(&file);
    ui->planillaExcel->setText(streamMO.readLine());

    QFile fileP(path+"/pathFuncionalidad.txt");
    fileP.open(QIODevice::ReadWrite);
    QTextStream streamP(&fileP);
    ui->funcionalidadExcel->setText(streamP.readLine());

    file.close();
    fileP.close();

    QThread* thread = new QThread(this);
    calc = new analizador(NULL);
    calc->moveToThread(thread);

    connect(ui->btnGO, &QPushButton::clicked, [=] () {calc->doHeavyCalculation(); } );
    connect(calc, SIGNAL(calculationCompleted(QStringList)), this, SLOT(imprimirSalida(QStringList)));
    connect(calc, &analizador::updateProgressbar, [=] (const int value) {ui->progressBar->setValue(value); } );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::buscarEnExcel(QString prc, QString fileExcelPlanilla)
{
    QXlsx::Document *docINP = new QXlsx::Document(fileExcelPlanilla);

    prc = prc.toLower();
    prc = prc.mid(0, prc.lastIndexOf("."));

    int nFilas = docINP->dimension().lastRow();
    for (int i = 2; i <= nFilas; ++i) {
        QString componente = docINP->read("A"+QString::number(i)).toString().toLower();
        QString relacion = docINP->read("B"+QString::number(i)).toString().toLower();

        if(componente==prc && relacion.contains(".wsdl")) {
            relaciones.append(relacion);
        }

    }

}

void MainWindow::buscarFuncionalidad(QString fileExcelFuncionalidad)
{
    QXlsx::Document *docINF = new QXlsx::Document(fileExcelFuncionalidad);
    docINF->selectSheet("Hoja2");

    int numFilas = docINF->dimension().lastRow();

    foreach (QString var, relaciones) {
        for (int i = 2; i <= numFilas; ++i) {
            QString endpoint = docINF->read("G"+QString::number(i)).toString().toLower();
            QString funcionalidad = docINF->read("E"+QString::number(i)).toString();
            if(endpoint.contains(var)) ui->textAreaSalida->appendPlainText(funcionalidad);
        }
    }
}


void MainWindow::imprimirSalida(QStringList lista)
{
    ui->textAreaSalida->appendPlainText(lista.join("\n"));
}


void MainWindow::on_actionEliminar_Duplicados_triggered()
{
    QStringList listaSalida = ui->textAreaSalida->toPlainText().split("\n");
    ui->textAreaSalida->clear();
    ui->statusBar->showMessage("INFO: Eliminando salidas duplicadas...", 3000);

    foreach (QString var, listaSalida)
    {
        listaSalida.removeAll(var);
        listaSalida.append(var);
    }
    ui->textAreaSalida->setPlainText(listaSalida.join("\n"));
}

void MainWindow::on_actionEliminar_configuraciones_triggered()
{
    QString FileFunc = QDir::currentPath()+"/config/pathFuncionalidad.txt";
    QString FilePlan = QDir::currentPath()+"/config/pathPlanilla.txt";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Test", "Estas seguro que deseas eliminar \nlas configuraciones guardadas?",QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        QFile fileF (FileFunc);
        fileF.remove();
        QFile fileP (FilePlan);
        fileP.remove();

        fileF.close();
        fileP.close();
    } else {
        qDebug() << "Yes was *not* clicked";
    }

}

void MainWindow::on_btnEditar_clicked()
{
    ui->btnSetear->setVisible(true);
    ui->btnEditar->setVisible(false);
    ui->textAreaEntrada->setEnabled(true);
    ui->btnGO->setEnabled(false);
    ui->btnSetear->setEnabled(true);
    ui->progressBar->setValue(0);
}

void MainWindow::on_btnSetear_clicked()
{
    if(!ui->textAreaEntrada->toPlainText().isEmpty())
    {
        if(!ui->planillaExcel->text().isEmpty() && !ui->funcionalidadExcel->text().isEmpty()){
            ui->textAreaSalida->clear();
            ui->btnSetear->setVisible(false);
            ui->btnEditar->setVisible(true);

            QString filePlanilla = ui->planillaExcel->text();
            QString fileFuncionalidad = ui->funcionalidadExcel->text();
            QStringList listaEntrada = ui->textAreaEntrada->toPlainText().split("\n");

            calc->setPath_to_planilla(filePlanilla);
            calc->setPath_to_funcionalidad(fileFuncionalidad);
            calc->setProcEntrada(listaEntrada);

            ui->textAreaEntrada->setEnabled(false);
            ui->btnSetear->setEnabled(false);
            ui->btnGO->setEnabled(true);

            //calc->doHeavyCalculation();

        }else{
            ui->statusBar->showMessage("ERROR: No se han cargado los archivos Excel.", 5000);
        }
    }else{
        ui->statusBar->showMessage("ERROR: Faltan los nombreas de los procedimientos.", 5000);
    }
}

void MainWindow::on_toolPlanilla_clicked()
{
    QString path,tipo,tipoFile;
    tipoFile = "xlsx";
    tipo = ( tipoFile.toUpper() )+" Files (*."+(tipoFile.toLower())+")";
    path = QFileDialog::getOpenFileName(this, "Abrir Archivo:", QDir::homePath(),tipo);

    if(path != NULL){
        ui->planillaExcel->setText(path);

        QString ruta = QDir::currentPath()+"/config";
        if(!QDir(ruta).exists())
            QDir().mkdir(ruta);

        QFile file(ruta+"/pathPlanilla.txt");
        if (file.open(QIODevice::WriteOnly)){
            QTextStream in(&file);
            in<<path;
        }
        file.close();
    }
}

void MainWindow::on_toolFuncionalidad_clicked()
{
    QString path,tipo,tipoFile;
    tipoFile = "xlsx";
    tipo = ( tipoFile.toUpper() )+" Files (*."+(tipoFile.toLower())+")";
    path = QFileDialog::getOpenFileName(this, "Abrir Archivo:", QDir::homePath(),tipo);

    if(path != NULL){
        ui->funcionalidadExcel->setText(path);

        QString ruta = QDir::currentPath()+"/config";
        if(!QDir(ruta).exists())
            QDir().mkdir(ruta);

        QFile file(ruta+"/pathFuncionalidad.txt");
        if (file.open(QIODevice::WriteOnly)){
            QTextStream in(&file);
            in<<path;
        }
        file.close();
    }
}
