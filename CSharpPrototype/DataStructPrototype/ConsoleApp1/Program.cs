// See https://aka.ms/new-console-template for more information
using System.ComponentModel;
using System.Collections.Generic;

Console.WriteLine("Hello, World!");

static int Add(int a, int b)
{
    return a + b;
}

Console.WriteLine(Add(5, 4));

int[][] NurseInfo = new int[2][];

// Initialize the first element with 5 numbers
NurseInfo[0] = new int[5];

// Initialize the second element with 42 numbers
NurseInfo[1] = new int[42];

// Create a Random object
Random rand = new Random();

// Loop through each row in the jagged array
for (int i = 0; i < NurseInfo.Length; i++)
{
    // Loop through each element in the current row
    for (int j = 0; j < NurseInfo[i].Length; j++)
    {
        // Set each element to a random number between 0 and 2
        NurseInfo[i][j] = rand.Next(0, 3); // 0 to 2 (upper bound exclusive)
    }
}

// Output the jagged array
for (int i = 0; i < NurseInfo.Length; i++)
{
    Console.WriteLine("Row " + i + ": " + string.Join(", ", NurseInfo[i]));
}

static int GetShiftPreference(int[][] info, int ShiftNum)
{
    return info[1][ShiftNum];
}

static int GetNurseNum(int[][] info)
{
    return info[0][0];
}



//dictionary for departments
Dictionary<int, string> Departments = new Dictionary<int, string>();

// Adding key/value pairs in the DictionaryUsing Add() method
Departments.Add(0, "Maternity");
Departments.Add(1, "Xray");
Departments.Add(2, "Emergency");

// Function to return the value for a given key
static string GetValueFromDictionary(Dictionary<int, string> dictionary, int key)
{
    if (dictionary.TryGetValue(key, out string? value))
    {
        return value;
    }
    else
    {
        return "Key not found";
    }
}

string GetNurseDept(int[][] info)
{
    int DeptNum = info[0][1];
    return GetValueFromDictionary(Departments, DeptNum);
}


Console.WriteLine("First 3 shift preferences:");
Console.WriteLine(GetShiftPreference(NurseInfo, 0));
Console.WriteLine(GetShiftPreference(NurseInfo, 1));
Console.WriteLine(GetShiftPreference(NurseInfo, 2));

Console.WriteLine("Nurse Number:");
Console.WriteLine(GetNurseNum(NurseInfo));

Console.WriteLine("Nurse Dept:");
Console.WriteLine(GetNurseDept(NurseInfo));

