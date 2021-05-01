# TXUtils
TXUtils - это библиотека, написанная мною для ~~себя~~ облегчения и удобства работы с графикой, используя [TXLib](http://storage.ded32.net.ru/Lib/TX/TXUpdate/Doc/HTML.ru/).



## Установка
Всё что вам потребуется, это скачать данный репозиторий и распаковать в любом удобном месте. Например в C:\Lib\TXUtils\.
Затем подключите файл TXU.h в вашей программе после подключения TXLIb:

```
#include <TXLib.h>
#include "C:\Lib\TXUtils\TXU.h"
```

Готово!

Примечание: я гарантирую совместимость лишь с компилятором vusial studio и не отвечаю за совместимость с другимисредами.

# Классы библиотеки:
- txu::Color
- txu::Font
- txu::Context

# Функции библиотеки:
- void txu::WasExitButtonPressed    ()
- int  txu::GetMouseWheel           ()
- void txu::SetConsoleOutputEnabled (bool enable)
- bool txu::IsConsoleOutputEnabled  ()


# txu::Color
Класс предназначеный для хранения и операций с цветом.
## Конструкторы класса:
- txu::Color ()
