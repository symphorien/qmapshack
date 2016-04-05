/**********************************************************************************************
   Copyright (C) 2015 Ivo Kronenberg

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

**********************************************************************************************/

#ifndef CFITSTATE_H
#define CFITSTATE_H

#include "gis/fit/decoder/CFitDefinitionMessage.h"
#include "gis/fit/decoder/CFitMessage.h"

#include <QtCore>


typedef enum
{
    eDecoderStateFileHeader,
    eDecoderStateRecord,
    eDecoderStateRecordContent,
    eDecoderStateFieldDef,
    eDecoderStateFieldData,
    eDecoderStateFileCrc,
    eDecoderStateEnd
} decode_state_e;

class IFitDecoderState
{
    Q_DECLARE_TR_FUNCTIONS(IFitDecoderState)
public:
    struct shared_state_data_t
    {
        quint16 crc;
        quint32 fileLength;
        quint32 fileBytesRead;
        quint8 lastTimeOffset;
        quint32 timestamp;
        CFitDefinitionMessage* lastDefintion;
        CFitMessage* lastMessage;
        QMap<quint8, CFitDefinitionMessage> defintions;
        QList<CFitDefinitionMessage> defintionHistory;
        QList<CFitMessage> messages;
    };

    IFitDecoderState(shared_state_data_t &data) : data(data) { }
    virtual ~IFitDecoderState() {}

    virtual void reset() = 0;
    decode_state_e processByte(quint8 &dataByte);

protected:
    virtual decode_state_e process(quint8 &dataByte) = 0;

    CFitMessage* latestMessage() const { return data.lastMessage; }
    void addMessage(const CFitDefinitionMessage& definition);

    void setFileLength(quint32 fileLength);
    void resetFileBytesRead();
    void incFileBytesRead();
    quint32 bytesLeftToRead();

    CFitDefinitionMessage* latestDefinition() const { return data.lastDefintion; }
    CFitDefinitionMessage* defintion(quint32 localMessageType);
    void addDefinition(const CFitDefinitionMessage &definition);
    void endDefintion();

    void setTimestamp(quint32 fullTimestamp);
    void setTimestampOffset(quint32 offsetTimestamp);
    quint32 getTimestamp() const { return data.timestamp; }
    quint16 getCrc() const { return data.crc; }

private:
    void buildCrc(quint8 byte);

    shared_state_data_t &data;
};

#endif // CFITSTATE_H