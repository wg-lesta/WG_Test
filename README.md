##Changelog

12.02.2015 Переписан и упрощен рендеринг объектов (убран QTый item-based renderer)

##Основные понятия

* **Прототипы объектов (game/prototypes.h).** Прототип представляет из себя имя и ассоциированный с ним набор свойств;

            {
                "name": "protoname",
                "properties":
                [
                    {
                        "name": "property",
                        "type": "int|float|bool|string|point",
                        "value": value,
                        "range": { "min":value, "max":value }
                    }
                ]
            }
            
* **Окружение (game/enviroment.h).** Описывает ресурсы и прототипы объектов;

            {
                "version": 1.0,
                "tileset": "path to image",
                "tilesize": {"width" : value, "height" : value},
                "prototypes":
                 [
                    {
                        "name": "protoname",
                        "properties":
                        [
                            {
                                "name": "property name",
                                "type": "int|float|bool|string|point",
                                "value": value
                                "range": { "min":value, "max":value }
                            }
                        ]
                    }
                ]
            }
            

* **Объекты (game/objects.h).** Объекты созданные на уровни, определяются своим прототипом;

            {
                "prototype": "object name from config",
                "properties":
                [
                    {"name": "property name", "value": "different from the default value"}
                ]
            }
            
* **Уровень (game/level.h).** Уровень это набор объектов созданный в определенном окружении;

            {
                "version": 1.0,
                "name": "levelname",
                "size": { "width": 10, "height": 10 },
                "objects":
                [
                    {
                        "prototype": "prototype name from enviroment",
                        "properties":
                        [
                            {"name": "property name", "value": "value"}
                        ]
                    }
                ]
            }


##Сборка

1. Добавить QTDIR в переменное окружение
2. Создать WG_test\build
3. Перейти в WG_test\build
4. cmake ..
5. Отврыть файл проекта build\WG_Test.sln

Собранный редактор находится в WG_Test\bin