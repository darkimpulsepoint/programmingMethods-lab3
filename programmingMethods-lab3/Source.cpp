#include <iostream>
#include <time.h>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;

string notIntNumbersMessage = "Для этой операции нужны целые числа!";
string incorrectInputMsg = "Некорректный ввод!";

class BigInt {

private:
    vector<int> digits;

    int findDiv(BigInt a, BigInt b) const {
        int res = 0;

        while (!(a < b)) {
            res++;
            a = a - b;
        }
        return res;
    }

public:
    BigInt(int number = 0) {

        while (number > 0) {
            digits.push_back(number % 10);
            number /= 10;
        }

    }

    BigInt operator+(const BigInt& b) const {
        BigInt result;

        int transfer = 0;
        for (int i = 0; i < max(digits.size(), b.digits.size()); i++) {
            int x = digits.size() <= i ? 0 : digits[i];
            int y = b.digits.size() <= i ? 0 : b.digits[i];
            result.digits.push_back((x + y + transfer) % 10);
            transfer = (x + y + transfer) / 10;
        }

        if (transfer != 0) {
            result.digits.push_back(transfer);
        }
        return result;
    }

    void removeRightZeros() {

        while (digits.back() == 0)
        {
            digits.pop_back();
            if (digits.empty())
                return;
        }
    }

    BigInt operator-(const BigInt& b) const {
        BigInt result;

        int transfer = 0;
        for (int i = 0; i < max(digits.size(), b.digits.size()); i++) {
            int x = digits.size() <= i ? 0 : digits[i];
            int y = b.digits.size() <= i ? 0 : b.digits[i];

            if (x - transfer < y) {
                result.digits.push_back(10 + x - y - transfer);
                transfer = 1;
            }
            else {
                result.digits.push_back(x - y - transfer);
                transfer = 0;
            }
        }
        result.removeRightZeros();
        return result;
    }

    bool operator<(const BigInt b) const {
        for (int i = max(digits.size(), b.digits.size()) - 1; i >= 0; --i) {
            int x = digits.size() <= i ? 0 : digits[i];
            int y = b.digits.size() <= i ? 0 : b.digits[i];

            if (x != y)
                return x < y;
        }
        return false;
    }

    BigInt operator/(const BigInt b) const {
        BigInt result, first;

        int tempDiv;
        for (int i = 0; i < digits.size(); i++) {
            first.digits.insert(first.digits.begin(), digits[digits.size() - i - 1]);
            tempDiv = findDiv(first, b);
            first = first - (b * BigInt(tempDiv));
            result.digits.insert(result.digits.begin(), tempDiv);
        }
        result.removeRightZeros();
        return result;
    }

    BigInt operator*(const BigInt& b) const {

        BigInt res = 0;

        int transfer = 0;

        for (int i = 0; i < b.digits.size(); i++) {
            transfer = 0;

            BigInt temp;

            for (int j = 0; j < i; j++) {
                temp.digits.push_back(0);
            }

            for (int j = 0; j < digits.size(); j++) {
                int value = digits[j] * b.digits[i] + transfer;
                transfer = value / 10;
                temp.digits.push_back(value - transfer * 10);
            }
            if (transfer != 0) {
                temp.digits.push_back(transfer);
            }
            res = res + temp;
        }
        return res;
    }

    string toString() {
        string str;
        if (digits.empty()) {
            str = "0";
        }
        else {
            for (int i = digits.size() - 1; i >= 0; i--) {
                str += to_string(digits[i]);
            }

        }
        return str;
    }
};



void printMsgIncorrectInput() {
    cout << incorrectInputMsg << endl;
}

bool checkIsInt(double num) {
    return (num - static_cast<int>(num) == 0);
}

BigInt factorial(BigInt num) {
    if (num < 0) {
        cout << "Нельзя посчитать факториал числа меньше нуля!" << endl;
        return 0;
    }

    BigInt res = 1;
    for (BigInt i = 1; i < BigInt(num) + 1; i = i + 1) {
        res = res * i;
    }

    return res;
}

void task1() {
    srand(time(NULL));

    int computerChoice = rand() % 3;
    int userChoice;

    cout << "Выберите:\n0 - камень\n1 - ножницы\n2 - бумага" << endl;
    cin >> userChoice;

    if (userChoice < 0 || userChoice > 2) {
        printMsgIncorrectInput();
        return;
    }

    cout << "Выбор компьютера: " << computerChoice << endl;

    string answers[] = { "ничья", "Вы", "компьютер" };

    cout << "Победитель: " << answers[(computerChoice - userChoice + 3) % 3] << endl;


}

void task2() {
    double firstNum, secondNum;
    char op;

    cout << "Первое число: " << endl;
    cin >> firstNum;

    cout << "Операция: (*, +, -, :, %, /)" << endl;
    cin >> op;

    string ops = "*+-:%/";

    if (ops.find(op) == string::npos) {
        printMsgIncorrectInput();
        return;
    }

    cout << "Второе число: " << endl;
    cin >> secondNum;

    double result;

    switch (op)
    {
    case '*':
        result = firstNum * secondNum;
        break;

    case '+':
        result = firstNum + secondNum;
        break;

    case '-':
        result = firstNum - secondNum;
        break;

    case ':':
        result = firstNum / secondNum;
        break;

    case '%':
        if (!(checkIsInt(firstNum) && checkIsInt(secondNum))) {
            cout << notIntNumbersMessage << endl;
            return;
        }
        result = static_cast<int>(firstNum) % static_cast<int>(secondNum);
        break;

    case '/':
        if (!(checkIsInt(firstNum) && checkIsInt(secondNum))) {
            cout << notIntNumbersMessage << endl;
            return;
        }
        result = (int)firstNum / (int)secondNum;
        break;

    default:
        cout << "Операция введена неверно!" << endl;
        break;
    }

    cout << firstNum << " " << op << " " << secondNum << " " << "=" << " " << result << endl;
}

void task3() {
    BigInt result = 0;
    BigInt n, k;

    string n_str;

    cout << "Введите n: " << endl;
    cin >> n_str;

    size_t screamerPos = n_str.find("!");
    bool isOneVar = false;

    if (screamerPos == string::npos) {
        n = BigInt(stoi(n_str));
    }
    else {
        n = BigInt(stoi(n_str.substr(0, screamerPos)));
        isOneVar = true;
    }

    if (isOneVar) {
        cout << "Факториал " << n.toString() << " равен " << factorial(n).toString() << endl;
    }
    else {
        cout << "Введите k: " << endl;
        int temp;
        cin >> temp;
        k = temp;

        if (n < k) {
            cout << "n не может быть меньше k !" << endl;
            return;
        }


        BigInt factN = factorial(n);
        BigInt factK = factorial(k);

        BigInt factNmK = factorial(n - k);

        BigInt permutation = factN / factNmK;
        BigInt combination = permutation / factK;


        cout << "Число размещений из " << n.toString() << " по " << k.toString() << " равно " << permutation.toString() << endl;
        cout << "Число сочетаний из " << n.toString() << " по " << k.toString() << " равно " << combination.toString() << endl;
    }
}

class Question {
private:
    string question;
    vector<int> correctAnswers;

public:
    Question(string q, vector<int>a) {
        question = q;
        correctAnswers = a;
    }

    int getPoints(vector<int> userAnswers) {
        int correctAnswersNumber = 0;
        for (int i : userAnswers) {
            correctAnswersNumber += count(correctAnswers.begin(), correctAnswers.end(), i) ? 1 : 0;
        }

        if (correctAnswersNumber == correctAnswers.size()) {
            return correctAnswers.size() == userAnswers.size() ? 2 : 1;
        }
        else if (correctAnswersNumber == 0) {
            return 0;
        }
        else return 1;
    }

    string getQuestion() {
        return question;
    }
};

void task4() {
    const int questionNumber = 8;

    Question questions[questionNumber] = {
        Question("Кто зам.декана на ММФ?\n1 - Курсов\n2 - Курсовой\n3 - Курсовая\n", {1}),
        Question("Какой этаж у мехмата?\n1 - 2 этаж\n2 - 3 этаж\n3 - 4 этаж\n", {3}),
        Question("Мехмат или ФПМИ?\n1 - ммф\n2 - фпми\n", {1}),
        Question("Что можно пропустить?\n1 - физру\n2 - историю\n3 - пару стаканчиков\n", {1, 2, 3}),
        Question("Васильев выгнал с лекции. Что делать?\n1 - роскомнадзор\n2 - сдаться\n3 - молиться и каяться\n4 - отчислиться и уйти в бгуир\n", {3}),
        Question("Верно ли, что ММФ - факультет любви, потому что преподаватели любят студентов?\n1 - да\n2 - нет\n", {1}),
        Question("Как известно, в математике идеал - это проблемная черная дыра. Верно ли, что ММФ является идеалом?\n1 - да\n2 - нет\n", {1}),
        Question("Без каких теорий нельзя обойтись на ММФ?\n1 - теорий заговоров\n2 - теории множеств\n3 - теории графов\n", {2, 3}),
    };

    cout << "Опросник про МехМат (и около того) \n" <<
        "Сейчас у вас спросят " << questionNumber << " вопросов. Ваша задача ответить на них." \
        "В каждом вопросе может быть более одного правильного ответа\n" <<
        "Желаем удачи!\n" << endl;


    int points = 0;

    for (int i = 0; i < questionNumber; i++) {
        int numberOfAnswers = 0;

        cout << "\nВопрос " << i + 1 << endl;
        cout << questions[i].getQuestion() << endl;

        cout << "Ответов вы дадите: " << endl;
        cin >> numberOfAnswers;
        cout << "Ваши ответы: " << endl;

        vector<int> userAnswers;
        for (int j = 0; j < numberOfAnswers; j++) {
            int answer;
            cin >> answer;
            userAnswers.push_back(answer);
        }

        points += questions[i].getPoints(userAnswers);
    }

    cout << "Вы набрали " << points << " баллов" << endl;

    double percent = double(points) / (questionNumber * 2);

    cout << "Результат: " << endl;

    if (percent > 0.75) {
        cout << "Легенда МехМата" << endl;
    }
    else if (percent > 0.5) {
        cout << "Опытный студент" << endl;
    }
    else if (percent > 0.25) {
        cout << "Перваш" << endl;
    }
    else {
        cout << "Абитура" << endl;
    }
}


int main() {
    setlocale(LC_ALL, "Russian");

    int taskNumber;

    void (*tasks[])() = { task1, task2, task3, task4 };

    while (true) {
        //        system("cls");


        cout << "Введите номер задания (1-4)" << endl;
        cout << "1 - камень-ножницы-бумага\n2 - калькулятор\n3 - сочетания, размещения, факториал\n4 - опросник\n0 - exit" << endl;
        cin >> taskNumber;

        if (taskNumber == 0) {
            exit(0);
        }
        else if (taskNumber > 0 && taskNumber < 5) {
            tasks[taskNumber - 1]();
        }
        else {
            printMsgIncorrectInput();
        }
        //        system("pause");
    }

    return 0;
}