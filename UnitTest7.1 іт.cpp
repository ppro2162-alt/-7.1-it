#include "pch.h"
#include "CppUnitTest.h"

// Підключаємо код лабораторної; main() перейменовуємо, щоб він не заважав
#define main lab_main
#include "C:\Users\USSER\Desktop\лаби\ап 7.1 іт\ап 7.1 іт\ап 7.1 іт.cpp"
#undef main

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest71it
{
    // ---------- допоміжні функції ----------
    static int** MakeMatrix(const int rowCount, const int colCount, const int* data)
    {
        int** a = new int* [rowCount];
        for (int i = 0; i < rowCount; i++)
        {
            a[i] = new int[colCount];
            for (int j = 0; j < colCount; j++)
                a[i][j] = data[i * colCount + j];
        }
        return a;
    }

    static void FreeMatrix(int** a, const int rowCount)
    {
        DeleteRows(a, rowCount);
        delete[] a;
    }

    static bool Equals(int** a, const int rowCount, const int colCount, const int* expected)
    {
        for (int i = 0; i < rowCount; i++)
            for (int j = 0; j < colCount; j++)
                if (a[i][j] != expected[i * colCount + j])
                    return false;
        return true;
    }

    static long long SumAll(int** a, const int rowCount, const int colCount)
    {
        long long s = 0;
        for (int i = 0; i < rowCount; i++)
            for (int j = 0; j < colCount; j++)
                s += a[i][j];
        return s;
    }

    // ---------- Criterion ----------
    TEST_CLASS(CriterionTests)
    {
    public:
        TEST_METHOD(PositiveMultiplesOf5_AreExcluded)
        {
            Assert::IsFalse(Criterion(5));
            Assert::IsFalse(Criterion(10));
            Assert::IsFalse(Criterion(25));
            Assert::IsFalse(Criterion(50));
        }

        TEST_METHOD(PositiveNonMultiples_AreIncluded)
        {
            Assert::IsTrue(Criterion(1));
            Assert::IsTrue(Criterion(4));
            Assert::IsTrue(Criterion(7));
            Assert::IsTrue(Criterion(51));
        }

        TEST_METHOD(Zero_IsIncluded)
        {
            Assert::IsTrue(Criterion(0));
        }

        TEST_METHOD(Negatives_AreIncluded)
        {
            Assert::IsTrue(Criterion(-1));
            Assert::IsTrue(Criterion(-5));
            Assert::IsTrue(Criterion(-10));
            Assert::IsTrue(Criterion(-42));
        }
    };

    // ---------- NeedChange ----------
    TEST_CLASS(NeedChangeTests)
    {
    public:
        TEST_METHOD(FirstColumnDecides)
        {
            int d1[] = { 5, 0, 0,   3, 0, 0 };
            int d2[] = { 3, 0, 0,   5, 0, 0 };
            int** a = MakeMatrix(2, 3, d1);
            int** b = MakeMatrix(2, 3, d2);
            Assert::IsTrue(NeedChange(a, 0));
            Assert::IsFalse(NeedChange(b, 0));
            FreeMatrix(a, 2);
            FreeMatrix(b, 2);
        }

        TEST_METHOD(SecondColumnDescending)
        {
            int d1[] = { 4, 1, 0,   4, 9, 0 };
            int d2[] = { 4, 9, 0,   4, 1, 0 };
            int** a = MakeMatrix(2, 3, d1);
            int** b = MakeMatrix(2, 3, d2);
            Assert::IsTrue(NeedChange(a, 0));
            Assert::IsFalse(NeedChange(b, 0));
            FreeMatrix(a, 2);
            FreeMatrix(b, 2);
        }

        TEST_METHOD(ThirdColumnAscending)
        {
            int d1[] = { 4, 7, 8,   4, 7, 2 };
            int d2[] = { 4, 7, 2,   4, 7, 8 };
            int** a = MakeMatrix(2, 3, d1);
            int** b = MakeMatrix(2, 3, d2);
            Assert::IsTrue(NeedChange(a, 0));
            Assert::IsFalse(NeedChange(b, 0));
            FreeMatrix(a, 2);
            FreeMatrix(b, 2);
        }

        TEST_METHOD(EqualKeys_NoChange)
        {
            int d[] = { 4, 7, 2, 100,   4, 7, 2, -100 };
            int** a = MakeMatrix(2, 4, d);
            Assert::IsFalse(NeedChange(a, 0));
            FreeMatrix(a, 2);
        }

        TEST_METHOD(UsesCorrectRowIndex)
        {
            int d[] = { 1, 0, 0,   2, 0, 0,   0, 0, 0 };
            int** a = MakeMatrix(3, 3, d);
            Assert::IsFalse(NeedChange(a, 0));
            Assert::IsTrue(NeedChange(a, 1));
            FreeMatrix(a, 3);
        }
    };

    // ---------- Change ----------
    TEST_CLASS(ChangeTests)
    {
    public:
        TEST_METHOD(SwapsTwoRows)
        {
            int d[] = { 1, 2, 3,   4, 5, 6,   7, 8, 9 };
            int e[] = { 1, 2, 3,   7, 8, 9,   4, 5, 6 };
            int** a = MakeMatrix(3, 3, d);
            Change(a, 1, 2, 3);
            Assert::IsTrue(Equals(a, 3, 3, e));
            FreeMatrix(a, 3);
        }

        TEST_METHOD(TwiceRestoresOriginal)
        {
            int d[] = { 1, 2, 3,   4, 5, 6 };
            int** a = MakeMatrix(2, 3, d);
            Change(a, 0, 1, 3);
            Change(a, 0, 1, 3);
            Assert::IsTrue(Equals(a, 2, 3, d));
            FreeMatrix(a, 2);
        }
    };

    // ---------- Sort ----------
    TEST_CLASS(SortTests)
    {
    public:
        TEST_METHOD(KnownMatrix)
        {
            int d[] = {
                 5,  1,  1,  0,
                 2,  3,  9,  0,
                 5,  7,  1,  0,
                 2,  3,  4,  0,
                -1,  0,  0,  0
            };
            int e[] = {
                -1,  0,  0,  0,
                 2,  3,  4,  0,
                 2,  3,  9,  0,
                 5,  7,  1,  0,
                 5,  1,  1,  0
            };
            int** a = MakeMatrix(5, 4, d);
            Sort(a, 5, 4);
            Assert::IsTrue(Equals(a, 5, 4, e));
            FreeMatrix(a, 5);
        }

        TEST_METHOD(AlreadySorted_Unchanged)
        {
            int d[] = { 1, 5, 0,   1, 3, 0,   2, 9, 0 };
            int** a = MakeMatrix(3, 3, d);
            Sort(a, 3, 3);
            Assert::IsTrue(Equals(a, 3, 3, d));
            FreeMatrix(a, 3);
        }

        TEST_METHOD(ReverseOrder)
        {
            int d[] = { 3, 0, 0,   2, 0, 0,   1, 0, 0 };
            int e[] = { 1, 0, 0,   2, 0, 0,   3, 0, 0 };
            int** a = MakeMatrix(3, 3, d);
            Sort(a, 3, 3);
            Assert::IsTrue(Equals(a, 3, 3, e));
            FreeMatrix(a, 3);
        }

        TEST_METHOD(SingleRow_NoCrash)
        {
            int d[] = { 1, 2, 3 };
            int** a = MakeMatrix(1, 3, d);
            Sort(a, 1, 3);
            Assert::IsTrue(Equals(a, 1, 3, d));
            FreeMatrix(a, 1);
        }

        TEST_METHOD(RandomMatrix_IsOrderedAndKeepsData)
        {
            const int R = 7, C = 6;
            int** a = new int* [R];
            for (int i = 0; i < R; i++)
                a[i] = new int[C];
            Create(a, R, C, -42, 51);

            long long sumBefore = SumAll(a, R, C);
            Sort(a, R, C);

            Assert::IsTrue(SumAll(a, R, C) == sumBefore);
            for (int i = 0; i < R - 1; i++)
                Assert::IsFalse(NeedChange(a, i));

            FreeMatrix(a, R);
        }
    };

    // ---------- Create ----------
    TEST_CLASS(CreateTests)
    {
    public:
        TEST_METHOD(ValuesWithinRange)
        {
            const int R = 7, C = 6, Low = -42, High = 51;
            int** a = new int* [R];
            for (int i = 0; i < R; i++)
                a[i] = new int[C];

            srand(12345);
            for (int attempt = 0; attempt < 50; attempt++)
            {
                Create(a, R, C, Low, High);
                for (int i = 0; i < R; i++)
                    for (int j = 0; j < C; j++)
                    {
                        Assert::IsTrue(a[i][j] >= Low);
                        Assert::IsTrue(a[i][j] <= High);
                    }
            }
            FreeMatrix(a, R);
        }

        TEST_METHOD(DegenerateRange_GivesConstant)
        {
            int** a = new int* [2];
            for (int i = 0; i < 2; i++)
                a[i] = new int[2];
            Create(a, 2, 2, 7, 7);
            int e[] = { 7, 7, 7, 7 };
            Assert::IsTrue(Equals(a, 2, 2, e));
            FreeMatrix(a, 2);
        }
    };

    // ---------- Calc ----------
    TEST_CLASS(CalcTests)
    {
    public:
        TEST_METHOD(KnownMatrix)
        {
            int d[] = {
                 5,  3, -5,
                10, -2,  0,
                 7, 15,  4
            };
            int e[] = {
                 5,  0,  0,
                10,  0,  0,
                 0, 15,  0
            };
            int** a = MakeMatrix(3, 3, d);
            int S = -1, k = -1;
            Calc(a, 3, 3, S, k);
            Assert::AreEqual(7, S);
            Assert::AreEqual(6, k);
            Assert::IsTrue(Equals(a, 3, 3, e));
            FreeMatrix(a, 3);
        }

        TEST_METHOD(ResetsAccumulatorsBeforeStart)
        {
            int d[] = { 1, 2,   3, 4 };
            int** a = MakeMatrix(2, 2, d);
            int S = 999, k = 999;
            Calc(a, 2, 2, S, k);
            Assert::AreEqual(10, S);
            Assert::AreEqual(4, k);
            FreeMatrix(a, 2);
        }

        TEST_METHOD(AllExcluded_NothingChanges)
        {
            int d[] = { 5, 10,   15, 20 };
            int** a = MakeMatrix(2, 2, d);
            int S = -1, k = -1;
            Calc(a, 2, 2, S, k);
            Assert::AreEqual(0, S);
            Assert::AreEqual(0, k);
            Assert::IsTrue(Equals(a, 2, 2, d));
            FreeMatrix(a, 2);
        }

        TEST_METHOD(AllIncluded_MatrixBecomesZero)
        {
            int d[] = { 1, -5,   0, 7 };
            int e[] = { 0, 0,   0, 0 };
            int** a = MakeMatrix(2, 2, d);
            int S = 0, k = 0;
            Calc(a, 2, 2, S, k);
            Assert::AreEqual(3, S);
            Assert::AreEqual(4, k);
            Assert::IsTrue(Equals(a, 2, 2, e));
            FreeMatrix(a, 2);
        }

        TEST_METHOD(SecondCall_FindsOnlyZeros)
        {
            int d[] = { 1, 5,   2, 10 };
            int** a = MakeMatrix(2, 2, d);
            int S = 0, k = 0;
            Calc(a, 2, 2, S, k);
            Assert::AreEqual(2, k);
            Calc(a, 2, 2, S, k);
            Assert::AreEqual(0, S);
            Assert::AreEqual(2, k);
            FreeMatrix(a, 2);
        }
    };
}