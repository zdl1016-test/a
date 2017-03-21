#include <stdio.h>
#include <vector>
#include <map>
#include <stdlib.h>

using namespace std;

void init_mat(vector< vector<int> >& mat)
{
    // 本地直接初始化为0
    for (uint32_t i=0; i<mat.size(); ++i)
        mat[i][i] = 0;

    mat[0][1] = 1;
    mat[0][2] = 3;
    mat[0][3] = 5;
    mat[0][4] = 5;
    mat[1][2] = 3;
    mat[1][3] = 5;
    mat[1][4] = 5;
    mat[2][3] = 1;
    mat[2][4] = 5;
    mat[3][4] = 5;

    // 初始化矩阵左下三角
    for (uint32_t i=1; i<mat.size(); ++i)
    {
        for (uint32_t j=0; j<i; ++j)
        {
            mat[i][j] = mat[j][i];
        }
    }
}

void print_mat(vector< vector<int> >& mat)
{
    for (uint32_t i=0; i<mat.size(); ++i)
    {
        for (uint32_t j=0; j<mat.size(); ++j)
        {
            printf("%d\t", mat[i][j]);
        }
        printf("\n");
    }
}

// 科目类型
enum
{
    ENUM_COURSE_LISTENING = 0,
    ENUM_COURSE_WRITING = 1,
    ENUM_COURSE_READING = 2,
};

// 校区
enum
{
    ENUM_SCHOOL_NANSHAN = 0,
    ENUM_SCHOOL_XINHE = 1,
    ENUM_SCHOOL_CBD = 2,
    ENUM_SCHOOL_SCIENCE= 3,
    ENUM_SCHOOL_BAOAN = 4,
};

class CTeacher
{
public:
    CTeacher()
    {
        Reset();
    }

    void Reset()
    {
        m_course = -1;
        m_free_times.clear();
    }

public:
    int m_course; // 教的科目类型
    vector<int> m_free_times; // 可上课时间
};

class CStudent
{
public:
    CStudent()
    {
        Reset();
    }

    void Reset()
    {
        m_pos = -1;
        m_free_times.clear();
        m_courses.clear();
    }

public:
    int m_pos; // 固定的上课校区
    vector<int> m_free_times; // 上课时间
    vector<int> m_courses; // 本周的课程
};

//void gen_schedules(vector<CTeacher> teachers, vector<CStudent> students, vector< vector<int> > mat, )
class C
class CSchedule
{
public:
    map<int, >
}

int main()
{
    const int school_num = 5;

    // 学校间距离的代价方阵
    vector< vector<int> > mat(school_num, vector<int>(school_num));

    init_mat(mat);
    print_mat(mat);

    vector<CTeacher> teachers;
    vector<CStudent> students;

    CTeacher tcher;
    tcher.m_course = ENUM_COURSE_LISTENING;
    tcher.m_free_times.push_back(11);
    tcher.m_free_times.push_back(12);
    tcher.m_free_times.push_back(13);

    teachers.push_back(tcher);

    CStudent stud;
    stud.m_pos = ENUM_SCHOOL_BAOAN;
    stud.m_courses.push_back(ENUM_COURSE_LISTENING);
    stud.m_courses.push_back(ENUM_COURSE_LISTENING);
    stud.m_free_times.push_back(12);

    return 0;
}
