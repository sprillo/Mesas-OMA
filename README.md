# Mesas-OMA

Programa para asignar los niveles a las mesas de los salones.

El programa lee del archivo in.txt

## Formato del input

ColumnasEnTotalEntreTodosLosSalones	CantidadDeSalones  

AlumnosDelNivel1 AlumnosDelNivel2 AlumnosDelNivel3  

CantidadDeColumnasDelSalon_1  
CantidadDeColumnasDelSalon_2  
...  
CantidadDeColumnasDelSalon_CantidadDeSalones  


CantidadDeMesasDeLaPrimerColumnaDelSalon_1 CantidadDeMesasDeLaSegundaColumnaDelSalon_1 ...  
CantidadDeMesasDeLaPrimerColumnaDelSalon_2 CantidadDeMesasDeLaSegundaColumnaDelSalon_2 ...  
...  
CantidadDeMesasDeLaPrimerColumnaDelSalon_CantidadDeSalones CantidadDeMesasDeLaSegundaColumnaDelSalon_CantidadDeSalones ...  

### Ejemplo (Nacional OMA 2015, La Falda)

45 6  

252 184 114  

9  
3  
10  
12  
6  
5  

5 20 20 20 22 21 22 22 21  
12 12 11  
11 12 12 10 12 12 11 12 9 8  
3 3 16 16 16 12 16 16 12 16 16 16  
8 8 7 6 7 7  
7 7 6 6 6  

#### Notar como es habitual considerar un salon fisico como dos salones logicos (partido por el medio, donde esta el jurado)

## Output

### Si hay solucion

Para cada columna de cada salon, muestra un numero de 1 a 3 indicando el nivel a poner ahi. Para el input anterior, se obtiene:

3 1 3 1 3 1 3 1 3  
1 3 2  
2 3 1 2 1 2 1 2 1 2  
1 2 1 2 1 2 1 2 1 2 1 2  
2 1 2 1 2 1  
2 1 2 1 2  

El programa no solo intenta acomodar los niveles. Si hay varias soluciones, se queda con una que ponga nivel 3 lo mas juno posible. Esto se hace para que sea mas facil acomodar los alumnos y repartir las pruebas de nivel 3. En el ejemplo anterior, nivel 3 se logro ubicar en tan solo 13 columnas. Notar que de un salon se salta al siguiente como si las columnas de esos dos salones fueran contiguas, asi que poner los salones en un orden razonable para que dos salones consecutivos esten cerquita.

### Si no hay solucion

Si hiciste alguna burrada con el input, como poner menos mesas que alumnos, te va a avisar (ojo, podes llegar a hacer otras burradas que no se va a dar cuenta). Si no encuentra cosas raras en el input, y no encontro solucion, simplemente te va a decir que no hay solucion.
