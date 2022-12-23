# Buzon-Inteligente

## Componentes

| Componente  | Unidades | Precio | Link |
|:---|:---:|--:|:--:|
| Wemos D1 Mini | 1 | 1,90€ | [Link](https://es.aliexpress.com/item/32688079351.html?spm=a2g0o.order_list.0.0.7d19194dl5ZCmY&gatewayAdapt=glo2esp) |
| Resistencias 10kΩ | 1 | <0,01€ | [Link](https://es.aliexpress.com/item/32636020144.html?spm=a2g0o.order_list.0.0.7d19194dl5ZCmY&gatewayAdapt=glo2esp)  |
| Microinterruptor | 2 | 0,20€ | [Link](https://es.aliexpress.com/item/32273125391.html?spm=a2g0o.order_list.0.0.7d19194dl5ZCmY&gatewayAdapt=glo2esp) |
| Caja para pilas AA | 1 | 0,20€ | [Link](https://es.aliexpress.com/item/4000980622098.html?spm=a2g0o.order_list.0.0.7d19194dl5ZCmY&gatewayAdapt=glo2esp) |
| Otros | -- | 0.20€ | -- |

Coste total: 2,50€

<br>

## Circuito
![alt text](https://github.com/drosell271/Buzon-Telegram/blob/main/Circuito/Circuito.png?raw=true)

El circuito se resumen en 2 componentes:
* Interruptor que detecta la llegada de una carta
* Interruptor que detecta la apertura de la puerta del buzón

<br>

1. Por otra parte el interruptor de detección de carta esta conectado de la siguiente manera: 
   - Común: GND + 10kΩ / Entrada digiral D5
   - NO (Normally Open): 3.3V (Activo cuando se pulsa)
   - NC (Normally Close): GND

2. Finalmente, el interruptor de detección de apertura de la puerta esta conectado de la siguiente manera: 
   - Común: GND + 10kΩ / Entrada digiral D6
   - NO (Normally Open): GND
   - NC (Normally Close): 3.3V (Activo cuando se deja de pulsar)

<br>

## Consumo
En este proyecto, vamos a usar el modo activo y el modo "light sleep". Dichos consumos son:
- Modo activo: 68 mA
- Light Sleep (o ahorro): 0.68 mA

### Consumo teórico
Consideraremos 2 eventos:
- Carta recibida: 15s en modo activo
- Carta recogida: 135s en modo activo

Consideraremos que un día de trabajo es 150s en modo activo (1 carta recibida + 1 carta recogida) y 86250s en modo ahorro.

Además tendremos en cuenta que correos trabaja de lunes a viernes, lo que nos deja con 5 días de trabajo y 2 de descanso cada semana.

A continuación un resumen de los cálculos detallados:

`Día de trabajo: (150/3600 · 68mA) + (86250/3600 · 0.68mA) = 19.125 mA/día`

`Día de descanso: (24 · 0.68mA) = 16.32 mA/día`

`Semana éstandar: (5 · 19.125mA) + (2 · 16.32mA) = 128.265 mA/semana`

Sabiendo que las pilas AA tiene una capacidad aproximada de 2500mA:

`Semanas de batería: 2500 / 128.265 = 19.49 semanas = 136 días = 4.5 meses`

<br>

## Montaje

### Componentes:
![alt text](https://github.com/drosell271/Buzon-Telegram/blob/main/Montaje/Componentes.png)


### Prototipo funcional:
![alt text](https://github.com/drosell271/Buzon-Telegram/blob/main/Montaje/Prototipo.png)


En progreso....
