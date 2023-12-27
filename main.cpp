#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <vector>

using namespace std;

// ������������ ��� ��� ������� ������
enum class TaskStatus
{
    NEW,         // �����
    IN_PROGRESS, // � ����������
    TESTING,     // �� ������������
    DONE         // ���������
};

// ��������� ���-������� ��� map<TaskStatus, int>,
// ������������ ������� ���������� ����� ������� �������
using TasksInfo = map<TaskStatus, int>;

class TeamTasks
{
public:
    // �������� ���������� �� �������� ����� ����������� ������������
    const TasksInfo &GetPersonTasksInfo(const string &person) const
    {

        return persons_.at(person);
    }

    // �������� ����� ������ (� ������� NEW) ��� ����������� �������������
    void AddNewTask(const string &person)
    {
        ++persons_[person][TaskStatus::NEW];
    }

    // �������� ������� �� ������� ���������� ����� ����������� ������������,
    // ����������� ��. ����
    tuple<TasksInfo, TasksInfo> PerformPersonTasks(const string &person, int task_count)
    {
        tuple<TasksInfo, TasksInfo> result;

        TasksInfo touched;
        TasksInfo untouched = persons_.at(person);
         
        for (int i = 0; i < task_count; ++i)
        {
            TasksInfo temp = persons_.at(person);
            for (const auto &[taskStatus, count] : temp)
            {
                if (count == 0)
                {
                    continue;
                }
                if (persons_.at(person)[taskStatus] >= 1)
                {
                    persons_.at(person)[taskStatus] -= 1;
                    untouched[taskStatus] -= 1;
                    if (untouched[taskStatus] <= 0)
                    {
                        untouched.erase(taskStatus);
                    }

                    touched[static_cast<TaskStatus>(static_cast<int>(taskStatus) + 1)] += 1;
                    persons_.at(person)[static_cast<TaskStatus>(static_cast<int>(taskStatus) + 1)] += 1;
                    break;
                }
            }
        }

        return tie(touched, untouched);
    }

private:
    map<string, TasksInfo> persons_;
};

// ��������� ������� �� ��������, ����� ����� �����������
// ���������� � ������������� ������ � ������� [] � �������� 0,
// �� ����� ��� ���� �������� �������.
void PrintTasksInfo(TasksInfo tasks_info)
{
    cout << tasks_info[TaskStatus::NEW] << " new tasks"s
         << ", "s << tasks_info[TaskStatus::IN_PROGRESS] << " tasks in progress"s
         << ", "s << tasks_info[TaskStatus::TESTING] << " tasks are being tested"s
         << ", "s << tasks_info[TaskStatus::DONE] << " tasks are done"s << endl;
}

int main()
{
    TeamTasks tasks;
    tasks.AddNewTask("Ilia"s);
    for (int i = 0; i < 3; ++i)
    {
        tasks.AddNewTask("Ivan"s);
    }
    cout << "Ilia's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ilia"s));
    cout << "Ivan's tasks: "s;
    PrintTasksInfo(tasks.GetPersonTasksInfo("Ivan"s));

    TasksInfo updated_tasks, untouched_tasks;

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);

    tie(updated_tasks, untouched_tasks) = tasks.PerformPersonTasks("Ivan"s, 2);
    cout << "Updated Ivan's tasks: "s;
    PrintTasksInfo(updated_tasks);
    cout << "Untouched Ivan's tasks: "s;
    PrintTasksInfo(untouched_tasks);
}