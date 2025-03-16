#ifndef ScaledInt_H
#define ScaledInt_H

#include "dataengineinterface.h"

class ScaledInt : public QObject, public DataEngineInterface
{
    Q_OBJECT
    Q_INTERFACES(DataEngineInterface)
    Q_PLUGIN_METADATA(IID "VOFA+.Plugin.ScaledInt")

public:
    explicit ScaledInt();
    ~ScaledInt();

    void ProcessingDatas(char *data, int count);
    bool ProcessingFrame(char *data, int count, QVector<float> &dd);
private:
    Frame *frame_;
};
#endif // ScaledInt_H
