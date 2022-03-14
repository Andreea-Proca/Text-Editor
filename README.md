# Text-Editor
 
Textul inserat este stocat intr-o lista dublu inlantuita, fiecare nod continand un caracter. 
Pentru a retine unde incepe fiecare linie din text am folosit un vector de pointeri la un nod din lista dublu inlantuita.
Pentru fiecare comanda posibila exista o functie care o efectueaza. Fiecare comanda este mai intai executata, iar apoi retinuta.
Comenzile sunt retinute ca siruri de caractere intr-o stiva de pointeri la siruri ce va fi folosita in cazul in care se doreste efectuarea operatiei "undo" pentru acea comanda.. 
Daca este efectuata operatia de "undo" pe o anumita comanda, aceasta este retinuta intr-o coada de pointeri la siruri ce va fi folosita in cazul in care se doreste efectuarea operatiei "redo" pentru acea comanda.
Pentru a sti pozitia curenta in text am utilizat o variabila de tip intreg care retine linia curenta si un pointer la nod care retine nodul curent.
Fiecare comanda citita este stocata intr-un sir de caractere din care se extrage tipul comenzii si argumentele ei. In functie de tip, este apelata functia corespunzatoare comenzii in care este efectuata, 
apoi este adaugata in stiva utilizata pentru comanda "undo", fie exact cum e, fie cu mai multe detalii cum ar fi pozitia la care a fost efectuata comanda.
Pentru comanda "undo" exista structuri decizionale pentru fiecare posibila comanda care efectueaza "undo" pentru acestea.
Dupa ce este efctuata, comanda este scoasa din stiva si stocata intr-o coada care retine comenzile prentu care s-a efectuat "undo" in cazul in care este necesara comanda "redo" pe acestea.
Comanda "redo" functioneaza dupa acelasi principiu, cu diferenta ca dupa ce este efectuata o comanda, aceasta este scoasa din coada.
In cazul comenziilor "save" si "quit", atata stiva, ca si coada sunt eliberate.
In plus, in cazul comenzii "save", continutul listei dublu-inlantuite este salvat in fisierul "editor.out".
