/*
 * @desc    : 简单的共享内存统计类
 * @author  : nemozhang
 * @history :
 *
 *    2012-05-21_11:41:00 Create
 *
 * @copyright: Shenzhen Tencent Co.Ltd
 *
 */
#ifndef _STAT_SIMPLE_H_
#define _STAT_SIMPLE_H_

#include <string>
#include <assert.h>

#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>           /* For O_* constants */
#include <asm/atomic.h>
#include <errno.h>

using namespace std;

class StatSimple
{
public:
    enum 
    {
        ENUM_WORK_MODE_SVR = 0,
        ENUM_WORK_MODE_CLI = 1
    };

    StatSimple(const char* shm_name, int item_num, int mode)
    {
        m_mode = mode;
        m_errmsg[0] = '\0';
        m_shm_name = shm_name;
        m_item_num = item_num;
        m_region_size = sizeof(atomic_t) * item_num;
        m_ptr = 0;
    }

    ~StatSimple()
    {
        if (m_ptr)
        {
            munmap(m_ptr, m_item_num * sizeof(atomic_t));
            shm_unlink(m_shm_name.c_str());
            m_ptr = 0;
        }
    }

    int Init()
    {
        int fd = shm_open(m_shm_name.c_str(), O_CREAT | O_RDWR, 0666);
        if (fd == -1)
        {
            snprintf(m_errmsg, sizeof(m_errmsg), "shm_open failed. errmsg: %s", strerror(errno));
            return -1;
        }

        if (ENUM_WORK_MODE_SVR == m_mode)
        {
            int r = ftruncate(fd, m_region_size);
            if (r != 0)
            {
                snprintf(m_errmsg, sizeof(m_errmsg), "ftruncate failed. errmsg: %s", strerror(errno));
                return -1;
            }
        }

        string filename = string("/dev/shm/") + m_shm_name;
        chmod(filename.c_str(), S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH);

        m_ptr = (char*)mmap(0, m_region_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        if (m_ptr == MAP_FAILED)
        {
            snprintf(m_errmsg, sizeof(m_errmsg), "mmap failed. errmsg: %s", strerror(errno));
            return -1;
        }
        close(fd);


        // 只有svr模式才需要初始化
        if (ENUM_WORK_MODE_SVR == m_mode)
        {
            // 原子变量初始化
            for (int i=0; i<m_region_size; ++i)
            {
                atomic_set((atomic_t*)&m_ptr[sizeof(atomic_t)*i], 0);
            }
        }

        return 0;
    }

    void Set(int index, int value)
    {
        assert(index < m_item_num);
        atomic_set((atomic_t*)&m_ptr[sizeof(atomic_t)*index], value);
    }
    int Get(int index)
    {
        assert(index < m_item_num);
        return atomic_read((atomic_t*)&m_ptr[sizeof(atomic_t)*index]);
    }

    void Add(int index, int value)
    {
        assert(index < m_item_num);
        atomic_add(value, (atomic_t*)&m_ptr[sizeof(atomic_t)*index]);
    }
    void Inc(int index)
    {
        Add(index, 1);
    }

    void Sub(int index, int value)
    {
        assert(index < m_item_num);
        atomic_sub(value, (atomic_t*)&m_ptr[sizeof(atomic_t)*index]);
    }
    void Dec(int index)
    {
        Sub(index, 1);
    }
    char* GetErrMsg()
    {
        return m_errmsg;
    }

private:
    string m_shm_name;
    char m_errmsg[512];
    char *m_ptr;
    int m_item_num;
    int m_region_size;
    int m_mode;
};

#endif
