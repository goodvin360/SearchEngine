# Реализация системы поиска документов
_______

## Назначение системы:
Поиск документов в базе по пользовательским запросам.

## Требования для работы с системой:
Проект работает с использованием `CMake` версии 3.23 или выше, и реализован с использованием стандарта `C++ 17`

Для своей работы проект использует вспомогательные библиотеки `nlohmann_json` (подключаются с использованием
`FetchContent` и поэтому для запуска потребуется интернет подключение), а также `GoogleTest` (файлы 
библиотеки находятся в директории проекта `gtests/lib`)

<!-- ## Краткое описание алгоритма поиска по запросу:
Поиск документов с использованием данной системы осуществляется с применением частотной библиотеки (словаря) и инвертированного индекса.
Частотная библиотека представляет собой ассоциативный контейнер, ключами которого являются уникальные слова из всей рассматриваемой базы документов. 
Значениями при ключах являются данные о частоте появления данного ключа в каждом из документов рассматриваемой базы. 

Например, рассмотрим базу из двух документов со следующим содержимым:

doc001: "milk", "sugar"

doc002: "sugar", "tree", "b"

В том случае содержимое частотной библиотеки будет следующим:

b: {1, 1}

milk: {0, 1}

sugar: {0, 1}, {1, 1}

tree: {1, 1}

Примечание: слова-ключи в библиотеке располагаются в алфавитном порядке, а нумерация документов начинается с нулевого значения. -->

## Работа с системой:
Задание области поиска (базы документов), а также формирование поисковых запросов осуществляется с использованием 
двух конфигурационных файлов: `config.json` и `request.json`

Указанные конфигурационные файлы располагаются в корне проекта.

Содержимое файла `config.json`:

```json
{
    "config": {
        "max_responses": "5",
        "name": "SkillboxSearchEngine",
        "version": "0.1"
    },
    "files": [
        "../resources/file001.txt",
        "../resources/file002.txt",
        "../resources/file003.txt"
    ]
}
```

В поле `config` указываются такие параметры системы как её название, версия, а также максимальное количество отображаемых результатов для одного поискового запроса. 

В поле `files` указаны адреса до документов, среди которых будет осуществляться поиск. Для удобства навигации предлагается предварительно размещать интересующие документы в директорию `resources`, находящуются в корневом каталоге проекта. 

В рамках одного запуска системы поиска документов, запросов может быть более одного. Все запросы перечисляются в файле `request.json`, структура которого показана ниже:

```json
{
    "requests": [
        "milk water",
        "sugar",
        "e milk"
    ]
}
```

Для удобства, в проекте реализована вспомогательная функция по генерации файлов `config.json` и `request.json` с требуемой внутренней архитектурой. По умолчанию вызов данной функции находится в `main.cpp` в закомментированном состоянии:

```c++
int main() {

//    createJsons();
```


## Возможности поисковой системы:
Система поиска предусматривает работу с документами объемом не более 1000 слов каждый, 
с максимальной длиной одного слова в документе в 100 символов. Слова размерами более 100 
символов будут исключены из поисковой базы. Если в документе содержится более 1000 слов, 
то для поиска в базу будет добавлена только первая тысяча слов из данного документа,
с отображением соответствующего предупреждающего сообщения.
Все слова в документах должны состоять из латинских букв и разделяются одним или 
несколькими пробелами.

Поисковых запросов, указываемых в файле `request.json` должно быть не более 1000 штук, 
при этом каждый из запросов может содержать от одного до десяти слов. Слова в запросе состоят 
из латинских букв и разделены одним или несколькими пробелами. Если в файле `request.json` 
приведено более 1000 запросов, для работы будет использоваться только первая тысяча из них.
Если в отдельном запросе указано более десяти слов, то для поиска будут использоваться только 
первые десять уникальных слов из данного запроса.

Алгоритм поиска документов не предусматривает обнаружение следующих орфографических 
символов: `, . : ! ? ;` поэтому не следует включать данные символы в запрос, с целью 
найти их в каком-либо документе.

Система поиска в документах по умолчанию работает с использованием возможностей 
многопоточности при составлении частотного словаря. При вызове функции обновления 
базы документов (`UpdateDocumentBase(std::vector<std::string> input_docs)`) каждый 
документ из базы передается для обработки в отдельный поток. По завершении обработки 
всех документов происходит формирование объединенного частотного словаря, 
используемого в дальнейшем для обработки поисковых запросов.

## Отображение результата выполнения поискового запроса
Результаты выполнения поискового запроса отображаются в файле `answers.json`, который 
генерируется в корневом каталоге проекта. Примерная структура файла показана ниже.

```json
{
  "answers": {
    "request001": {
      " result": true,
      "relevance": [
        {
          "docID": 1,
          "rank": 1.0
        },
        {
          "docID": 0,
          "rank": 0.25
        }
      ]
    },
    "request002": {
      " result": true,
      "docID": 1,
      "rank": 1.0
    },
    "request003": {
      " result": false
    }
  }
}
```

Результаты работы системы приводятся в соответствии со структурой файла с запросами. Первому
запросу из файла 
`request.json` соответствует результат поиска с обозначением `request001` и т.д. В случае, 
если по запросу найден хотя бы один документ с ненулевой релевантностью, в поле `result` 
записывается значение `true`. В противном случае - `false`.
Если по результатам запроса найдено более одного документа, то прописывается 
дополнительное поле `relevance`. Структурно результат к каждому запросу представляет 
собой пару (или ряд пар) значений: номер документа (его ID) и соответствующее значение 
относительной релевантности запроса для данного документа.
Результаты приводятся в порядке убывания относительной релевантности. В случае, если
ряд документов имеет одинаковое значение относительной релевантности, 
они выводятся в файл `answers.json` в порядке возрастания их ID.

## Автоматическое тестирование функционльных возможностей системы:
В проекте предусмотрено проведение модульного тестирования основных функциональных 
элементов: проверка составления частотного словаря, а также результатов работы самой 
поисковой системы.

Тестирование осуществляется с использованием возможностей библиотеки `Google Test`. 
Таргетом для запуска тестирования является `GoogleTest_run`.

