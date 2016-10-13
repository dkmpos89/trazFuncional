#ifndef ANALIZADOR_H
#define ANALIZADOR_H

#include <QObject>

class analizador : public QObject
{
    Q_OBJECT
public:
    explicit analizador(QObject *parent = 0);
    void buscarEnExcel(QString prc, QString fileExcelPlanilla);
    void buscarFuncionalidad(QString fileExcelFuncionalidad);

    QStringList getRelaciones() const;
    void setRelaciones(const QStringList &value);

    QString getPath_to_planilla() const;
    void setPath_to_planilla(const QString &value);

    QString getPath_to_funcionalidad() const;
    void setPath_to_funcionalidad(const QString &value);

    QStringList getProcEntrada() const;
    void setProcEntrada(const QStringList &value);

signals:
    void calculationCompleted(QStringList);
    void updateProgressbar(int);
public slots:
    void doHeavyCalculation();
private:
    QStringList relaciones;
    QString path_to_planilla;
    QString path_to_funcionalidad;
    QStringList procEntrada;
};

#endif // ANALIZADOR_H
