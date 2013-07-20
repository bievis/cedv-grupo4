Juego Ransom
============   
El juego consiste en liberar a los rehenes sin ser interceptado por los enemigos.
El juego está implementado bajo licencia GNU GPL v2.

Ejecución
=========
En la carpeta "bin" se encuentra los ejecutables del juego y la carpeta "media" que 
contiene los elementos multimedia que se manejan en el Juego.
- Ejecución en Linux, basta con entrar en la carpeta "bin" y ejecutar el
binario "ransom". 
- Ejecución en Windows, basta con entrar en la carpeta "bin" y ejecutar el
binario "Ransom.exe". 

Desglose de Carpetas
====================
\
 \
  -- blender
  |
  -- config
  |
  -- doc
  |
  -- include
  |
  -- media
  |
  -- others
  |
  -- Ransom
  |
  -- src
  |
  -- videos

- blender : carpeta con los modelos usados en el juego y realizados usando la herramienta blender.
- config : carpeta con el fichero de configuración del juego.
- doc : carpeta con la documentación de anteproyecto, proyecto y desarrollo (doxygen). 
- include : carpeta con los headers usados en el juego.
- media : carpeta con la configuracion media del juego, tales como sonidos, overlays, fondos, ...
- others : carpeta con todos los minijuegos realizados durante el CEDV 2012/2013 por del Grupo 4.
- Ransom : carpeta con todo lo necesario para el binario para Windows. Proyecto del Visual C++ 2010 Express.
- src : carpeta con del codigo fuente del juego.
- videos : videos extraidos del juego.

Menú
====
- START GAME: Opción para iniciar el juego. Si se quiere interrumpir el juego pulsar
la tecla ESC, donde aparecerá un menú con 3 opciones (Resume Game, Return to Main Menu y
Exit Game).
  - Teclas:
    - ARRIBA: Mueve el heroe hacia adelante.
    - ABAJO: Mueve el heroe hacia atras.
    - IZQUIERDA: Rota el heroe hacia la izquierda.
    - DERECHA: Rota el heroe hacia la derecha.
    - ESPACIO: Disparo del personaje.
    - O: Muestra/Oculta un panel de información.
- BEST TIMES: Ventana en la que se mostraran los 10 mejores tiempos que se han
obtenido en el juego, reflejando el tiempo empleado, ademas de la fecha y hora en la que 
realizó el registro. Para cerrar la ventana y volver al menú te tendrá que pulsar 
la tecla ESC.
- CREDITS: Ventana en la que se mostraran los nombres del equipo de desarrollo. Para 
cerrar la ventana y volver al menú tendrá que pulsar la tecla ESC.
- EXIT: Sale del juego.

Documentación
=============
La documentación (diagramas de clases, metodos y atributos de los objetos, etc..) 
los fuentes de encuentra en la carpeta "doc/devel" abriendo index.html con un 
navegador web.
La documentacion escrita se encuentra en la carpeta "doc".

Repositorio publico
===================
https://cedv-grupo4.googlecode.com/svn/trunk/ransom

Enlaces a videos en Youtube
===========================

Los videos se encuentran subidos en youtube, y sus enlaces son:

1.- Video realizando un rescate:

https://www.youtube.com/watch?v=S8lqoS4JyTs

2.- Video mostrando como los enemigos persiguen al heroe:

https://www.youtube.com/watch?v=ymCPeecnqQs

3.- Video realizando el rescate completo:

https://www.youtube.com/watch?v=-sguJKUVdFY

4.- Video mostrando como el enemigo nos derrota:

https://www.youtube.com/watch?v=47XbcY5GN6Q
