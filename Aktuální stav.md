1. **Přepínání scén za běhu (Ano/Ne) (kde a jak)**  
   Ano, je implementováno přes třídu `Scene`, která drží několik `DrawableObject` a jeden `Light`. Má na sobě metodu `render()` a vždy se ve scéně volá `render()` pouze jedné scény.
   1. **Základní scéna (Ano/Ne)** -- Ano
   2. **Scéna s lesem (Ano/Ne)** -- Ano
   3. **Scéna se čtyřmi kuličkami (Ano/Ne)** -- Ano
   4. **Scéna pro demonstraci použití všech shaderů (Konstantní, Lambert, Phong a Blinn) (Ano/Ne)** -- Ano

2. **Světlo (Ano/Ne) (kde a jak máte naimplementováno, jak se updatuje změna světla)**  
   Ano, je implementováno přes třídu `Light` a `PositionedLight` a přes observer pattern volá vždy `update()` na `ShaderProgram`.

3. **Základní třídy (ShaderProgram, DrowableObject, Camera, Controller) (Ano/Ne) (kdo zodpovídá za vykreslování, kde jsou uloženy modely, shadery atd.)**  
   Ano, modely jsou ve třídě `Model` a `DrawableObject` vždy na něj a na `ShaderProgram` drží smart pointer.

4. **Transformace (Composite pattern) (Ano/Ne) (Máte pro transformace základní třídy? Co a jak jste použili?)**  
   Transformace se nachází v namespace `transf` a mají všechny rodičovskou abstraktní třídu `Transformation`. `DrawableObject`, který tyto transformace pro každý objekt drží, při každém volání na `render()` tyto transformace aplikuje.

5. **Základy OOP**
   1. **Encapsulation (zapouzdření) (Ano/Ne) (Kde a jak?)**  
      Ano, žádná třída nemá getter a setter zároveň.

   2. **Inheritance (dědičnost) (Ano/Ne) (Kde a jak?)**  
      Většina dědičnosti v projektu je kvůli polymorfismu nebo OOP návrhovým vzorům.

   3. **Polymorphism (polymorfismus neboli mnohotvárnost) (Ano/Ne) (Kde a jak?)**  
      Ano, u `Light` a `Transformation`.

6. **Vertex a fragment shadery prosím uložte do textových souboru a použijte přiložený ShaderLoader pro jejich načítání (Ano/Ne) (Kde a jak jste použili?)**  
   Jaký přiložený ShaderLoader?

