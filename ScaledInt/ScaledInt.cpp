#include "ScaledInt.h"

ScaledInt::ScaledInt()
{
    frame_list_.append(Frame());
    frame_ = &frame_list_[0];
}

ScaledInt::~ScaledInt()
{

}

bool ScaledInt::ProcessingFrame(char *data, int count, QVector<float> &dd)
{
    if (count <= 0)
        return false;

    if (count % 4 == 0) {
        // 只有数据长度是4的倍数，才判定为标幺值数据
        for (int i = 0; i < count - 4; i += 4) {
            int value;
            // 调换字节序
            value = (data[i] & 0xff) << 24 | ((data[i + 1] & 0xff) << 16) | ((data[i + 2] & 0xff) << 8) | ((data[i + 3] & 0xff));
            // int转为float
            float value_float = static_cast<float>(value);
            dd.append(value_float);
        }
        return true;
    }
    return false;
}

// 帧结构：0x8A00XX5A 大端标幺值数组 0x5A00008A
// 其中XX表示数组数据长度，若帧头帧尾以及数组长度均匹配，则认为该帧为有效帧
// 大端标幺值数组类型为4字节32位有符号整型
void ScaledInt::ProcessingDatas(char *data, int count)
{
    frame_list_.clear();

    // 检测帧头及数据长度是否匹配
    int length = static_cast<unsigned char>(data[2]);
    // data[0]转换为无符号整型，判断是否为0x8A
    if(count != length * 4 + 8 || static_cast<unsigned char>(data[0]) != 0x8A || data[1] != 0x00 || data[3] != 0x5A)
        return;

    int begin = 0, end = 0;
    for (int i = 3; i < count; i++) {
        char *data_ptr = data + i - 3;
        int frame_tail_data;
        bool frame_is_valid = false;

        memcpy(&frame_tail_data, data_ptr, 4);
        if (frame_tail_data != static_cast<int>(0x8A00005A))
            continue;

        // 已经匹配到帧尾 0x00005A8A
        end = i;

        Frame frame;

        // 解析纯标幺值数据
        frame_is_valid = ProcessingFrame(data + begin + 4, (length + 1) * 4, frame.datas_);

        // 记录帧 是否合法，开始位置，结束位置
        frame.is_valid_ = frame_is_valid;
        frame.start_index_ = begin;
        frame.end_index_ = end;
        frame_list_.append(frame);
        // !记录帧

        begin = i + 4;

    }   
}

// void ScaledInt::ProcessingDatas(char *data, int count)
// {
//     frame_list_.clear();

//     int begin = 0, end = 0;
//     for (int i = 3; i < count; i++) {
//         char *data_ptr = data + i - 3;
//         int frame_tail_data;
//         bool frame_is_valid = false;

//         memcpy(&frame_tail_data, data_ptr, 4);
//         if (frame_tail_data != static_cast<int>(0x7F800000))
//             continue;

//         // 已经匹配到帧尾 0x00005A8A
//         end = i;

//         Frame frame;

//         // 解析纯标幺值数据
//         frame_is_valid = ProcessingFrame(data + begin, (i - begin) + 1, frame.datas_);

//         // 记录帧 是否合法，开始位置，结束位置
//         frame.is_valid_ = frame_is_valid;
//         frame.start_index_ = begin;
//         frame.end_index_ = end;
//         frame_list_.append(frame);
//         // !记录帧

//         begin = i + 4;

//     }   
// }
