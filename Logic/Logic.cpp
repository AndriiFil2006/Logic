#include <iostream>
#include <vector>
#include <string>
#include <functional>
#include <codecvt>

class ActualLogicStuff
{
public:
    //Straight-forward operations
    bool NotOperator(bool event, int numOfOperations = 1)
    {
        if (numOfOperations % 2 == 0)
        {
            return event;
        }
        return !event;
    }

    bool AndOperator(bool event1, bool event2)
    {
        return (event1 && event2);
    }

    bool OrOperator(bool event1, bool event2)
    {
        return (event1 || event2);
    }

    bool XOrOperator(bool event1, bool event2)
    {
        return ((event1 || event2) && !(event1 && event2));
    }

    bool ConditionalsOperator(bool event1, bool event2)
    {
        return (event2 || !event1);
    }

    bool BiConditionalsOperator(bool event1, bool event2)
    {
        return (ConditionalsOperator(event1, event2) && ConditionalsOperator(event2, event1));
    }

    //Creates a truth truth table (obvious from the name)
    std::pair<std::vector<std::wstring>, std::vector<std::vector<bool> > > CreateATruthTable(std::vector < std::wstring> events)
    {
        int numOfRows = pow(2, events.size()); // Calculates the number of rows
        std::vector<std::vector<bool> > TruthTable;

        for (int i = 0; i < events.size(); i++)
        {
            int numOfTruths = numOfRows / pow(2, i + 1); //How many truths are going to be itterating
            bool currBool = true;
            std::vector<bool> currRow;
            for (int j = 0; j < numOfRows / numOfTruths; j++)
            {
                for (int k = 0; k < numOfTruths; k++)   //Creating an initial table
                {
                    currRow.push_back(currBool);
                }
                currBool = !currBool;
            }
            TruthTable.push_back(currRow);
        }

        return std::make_pair(events, TruthTable);
    }

    std::pair<std::vector<std::wstring>, std::vector<std::vector<bool> > > AddOperationToATruthTable(
    std::pair<std::vector<std::wstring>, std::vector<std::vector<bool> > > TruthTable,
    std::wstring operation,
    std::function<bool(std::vector<bool>)> func)
    {
        TruthTable.first.push_back(operation);

        std::vector<bool> newColumn;
        for (int i = 0; i < TruthTable.second[0].size(); i++)
        {
            std::vector<bool> row;
            for (int j = 0; j < TruthTable.second.size(); j++)
            {
                row.push_back(TruthTable.second[j][i]);
            }
            newColumn.push_back(func(row));
        }

        TruthTable.second.push_back(newColumn);
        return TruthTable;
    }


    //Prints a given truth table (obvious from the name)
    void PrintTruthTable(std::pair<std::vector<std::wstring>, std::vector<std::vector<bool> > > TruthTable)
    {
        //Print header
        for (int i = 0; i < TruthTable.first.size(); i++)
        {
            std::wcout << TruthTable.first[i] << "\t";
        }
        std::cout << std::endl << std::endl;

        //Print Truth Table Column-Row Order
        for (int j = 0; j < TruthTable.second[0].size(); j++)
        {
            for (int i = 0; i < TruthTable.second.size(); i++)
            {
                int expressionlength = TruthTable.first[i].size();
                std::cout << ConvertBoolToString(TruthTable.second[i][j]);
                for (int k = 0; k < expressionlength; k++) //Offsetting stuff, I wish there was a better way to separate them tho
                {
                    std::cout << " ";
                }
                std::cout << "\t";
            }
            std::cout << std::endl;
        }
    }

    std::string ConvertBoolToString(bool val)
    {
        if (val)
        {
            return "T";
        }
        return "F";
    }
};

int main()
{
    std::locale::global(std::locale("en_US.UTF-8"));
    std::wcout.imbue(std::locale());



    //Testing stuff. Use it as a template

    ActualLogicStuff logic;
    /*
    std::cout << logic.NotOperator(false, 54) << std::endl;
    std::cout << logic.AndOperator(false, true) << std::endl;
    std::cout << logic.OrOperator(true, true) << std::endl;
    std::cout << logic.XOrOperator(false, false) << std::endl;
    std::cout << logic.ConditionalsOperator(true, false) << std::endl;
    std::cout << logic.BiConditionalsOperator(false, true) << std::endl;
    */

    std::pair<std::vector<std::wstring>, std::vector<std::vector<bool> >> truthTable = logic.CreateATruthTable({ L"p", L"q", L"r", L"s"});

    //Basic Example
    auto expression1 = [&logic](std::vector<bool> row) {
        bool p = row[0];
        bool q = row[1];
        return logic.BiConditionalsOperator(p, q);
        };
    truthTable = logic.AddOperationToATruthTable(truthTable, L"p ↔ q", expression1);


    // Example2: (p ∧ q) ∨ (p → q)
    auto expression2 = [&logic](std::vector<bool> row)
        {
            bool p = row[0];
            bool q = row[1];
            bool r = row[2];
            return logic.OrOperator(logic.AndOperator(p, q), logic.ConditionalsOperator(p, r));
        };

    truthTable = logic.AddOperationToATruthTable(truthTable, L"(p ∧ q) ∨ (p → r)", expression2);

    
    //example3: from homework 2, problem 1, part e) (q → p ⊕ r) ∨ ((q ∧ s) → (p ∨ r))
    auto expression = [&logic](std::vector<bool> row) {
        bool p = row[0];
        bool q = row[1];
        bool r = row[2];
        bool s = row[3];
        return logic.OrOperator(logic.ConditionalsOperator(q, logic.XOrOperator(p, r)), logic.ConditionalsOperator(logic.AndOperator(q, s), logic.OrOperator(p, r)));
        };


    truthTable = logic.AddOperationToATruthTable(truthTable, L"(q → p ⊕ r) ∨ ((q ∧ s) → (p ∨ r))", expression);
    logic.PrintTruthTable(truthTable);

    //Input way of expressions sucks ik, probably will fix that later. 
}
