## Distance Between two points

$$
Distance = \sqrt{{(y - y_0)}^{2} + {(x - x_0)}^{2}} \\
Angle = \arctan{(\frac{y - y_0 }{x - x_0})}
$$

## Ants moving

$$
ant\rightarrow x += ant\rightarrow speed \times \cos{(angle)} \\
ant\rightarrow y += ant\rightarrow speed \times \sin{(angle)}
$$

## Bouncing-off

When an ant hit a wall it goes in the opposite direction so
$$ Angle = Angle + \pi $$
and then it bounces with a 45 degree
$$ Angle = Angle \pm \frac{\pi}{4} $$

```c
if (ant->x + ANT_SIZE / 2 >= SCREEN_WIDTH || ant->x - ANT_SIZE / 2 <= 0)
{
    if (ant->x + ANT_SIZE / 2 >= SCREEN_WIDTH)
        ant->x = SCREEN_WIDTH - ANT_SIZE / 2;
    else if (ant->x - ANT_SIZE / 2 <= 0)
        ant->x = ANT_SIZE / 2;
    ant->angle = ant->angle + bounce[rand() % 2];
}
if (ant->y + ANT_SIZE / 2 >= SCREEN_HEIGHT || ant->y - ANT_SIZE / 2 <= 0)
{
    if (ant->y + ANT_SIZE / 2 >= SCREEN_HEIGHT)
        ant->y = SCREEN_HEIGHT - ANT_SIZE / 2;
    else if (ant->y - ANT_SIZE / 2 <= 0)
        ant->y = ANT_SIZE / 2;
    ant->angle = ant->angle + bounce[rand() % 2];
}
```

## Phoromone Release

$$ Phoromone \propto {1 \over Distance} $$
I want the Phormone to range from 0 to 100, and when ever the ant is about 100 pixels away from the food I want it to release 70

$$
\displaylines{
Phormone = 100 * \exp(- k * 100) \\
70 = 100 * \exp(- k * 100) \\
0.7 = \exp(- k * 100) \\
\ln{0.7} = -k * 100  \\
k = - {ln{0.7} \over 100} \\
k = 0.00356674944 \\
}
$$
