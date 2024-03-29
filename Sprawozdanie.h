/**
\mainpage Samotnik - dokumentacja i sprawozdanie

\section temat Temat

Napisać program do gry w samotnika. Pole do gry w samotnika ma kształt krzyża 
z jednym pustym polem w środku i 32 polami zapełnionymi. Jedynym dozwolonym
ruchem jest przeskoczenie pionka innym pionkiem w pionie lub w poziomie, przez co
przeskoczony pionek zostaje zbity. Celem gry jest pozostawienie na planszy jednego
pionka, najlepiej, jeśli będzie to pionek w centrum planszy.

\section analiza_projektu Analiza projektu 

Jako że postawionym przed autorem problemem był projekt gry planszowej, 
naturalnym rozwiązaniem zdało się podążanie za paradygmatem programowania obiektowego. 
W związku z faktem, iż język C nie posiada wbudowanych mechanizmów wspierających programowanie
obiektowe, zdecydowano się na ich samodzielną implementację. 

Kolejnym, nie mniej istotnym, krokiem na etapie projektowania był wybór architektury programu.
W tym względzie zdecydowano się na tzw. MVC (ang. Model-View-Controller), konwencję polegajacą na podziale aplikacji 
na trzy, możliwie niezależne, części, odpowiadające za odpowiednio: wewnętrzny stan programu (Model), warstwę prezentacji (View)
oraz interakcję z użytkownikiem i koordynację pracy programu (Controller). 

Ostatnią kluczową decyzją projektową był wybór biblioteki, umożliwiającej realizację warstw widoku i kontrolera. 
Ostatecznie wybrana została biblioteka GTK+ w wersji 3, umożliwiająca konstrukcję interfejsu użytkownika na bazie
plików XML. 

https://www.gtk.org/

Do generowania większości plików XML posłużono się programem Glade, zintegrowanym z biblioteką GTK+.

https://glade.gnome.org/

Poniżej zaprezentowano rozwiązania i konwencje stosowane w kodzie źródłowym, mające na celu
jak najlepsze oddanie poszczególnych popularnych koncepcji obiektowych, a także pomocne przy budowaniu
architektury MVC.


\subsection klasy_i_struktury Klasy i struktury

W języku C nie występuje koncepcja klasy, dobrze znana w językach wyższego poziomu. Co prawda pozwala on na definiowanie
struktur, a więc obiektów grupujacych dane, lecz występująca w projekcie konieczność dynamicznego alokowania pamięci
na niektóre z nich, obniżała czytelność i wprowadzała deorganizację kodu źródłowego. Celem ujednolicenia wykorzystywanych
rozwiązań podjęto kilka istotnych kroków:
    - Każda zdefiniowana struktura posiada alias, w postaci swojej oryginalnej nazwy, pozbawiony jednak członu 'struct'
       oraz zapisany w notacji PascalCase;
    - Struktury podzielono na reprezentujące klasy (a więc nie tylko posiadające stan, ale też enkapsulujące pewne zachowania), zawsze alokowane dynamicznie, nazywane dalej klasami - oraz na grupujące dane, alokowane na stosie.
    - Każda klasa posiada zdefiniowane metody *new* oraz *destroy*, odpowiednio alokujące i dealokujące wykorzystywane przez
       dany obiekt zasoby oraz pamięć;
    - Typy stanowiące prostą agregację danych musza posiadać jedynie metodę *create*, która, w odróżnieniu od *new*, zwracającego wskaźnik na dynamicznie alokowaną pamięć, tworzy obiekt na stosie, zwracając jego kopię.


\subsection metody Metody

Istotną częścią koncepcji klasy jest reprezentowanie charakterystycznych dla niej zachowań poprzez przyporządkowany jej zestaw
funkcji nazywanych metodami. W wielu językach programowania (np. w C++, C#, Javie) składnia wywołania metody jest następująca:
    
    obiekt.metoda(argumenty).

Osiągnięcie podobnej składni w C byłoby możliwe poprzez zdefiniowanie w obrębie klasy/struktury wskaźników na funkcje,
których pierwszym argumentem wywołania byłby zawsze obiekt nań wskazujący, miałoby to jednak niekorzystny wpływ na czytelność kodu.

Rozwiązanie wykorzystane w projekcie prezentuje się następująco:
    - Każdy plik nagłówkowy deklarujący klasę, deklaruje także zestaw funkcji prefiksowanych nazwą tejże klasy. 
       W przypadku ogólnym: NazwaKlasy_nazwaMetody;
    - Pierwszym argumentem tak zdefiniowanych funkcji jest zawsze wskaźnik na obiekt, którego zachowanie mają one reprezentować
       (wyjątek stanowi metoda *new*, która ma za zadanie ten obiekt stworzyć), 
       np. nagłówki wspomnianych wyżej metod *new* oraz *destroy* dla klasy Board prezentują się następująco:
       Board_new(), Board_destroy(Board* self);

\subsection pola_i_metody_prywatne Pola i metody prywatne

Pola i metody prywatne służą obiektom do wykonywania wewnętrznych operacji, a więc o charakterze wrażliwym bądź nieistotnym
z punktu widzenia użytkownika. Aby osiągnąc podobną funkcjonalność poczyniono następujące kroki:
    - Pliki nagłówkowe, miast definiować, jedynie deklarują klasy - definicje zawarte są w komplementarnych plikach źródłowych.
       Dzięki takiemu rozwiązaniu, pola klas, które powinny być dostępne z zewnątrz mogą zostać upublicznione przy pomocy
       tzw. akcesorów, czyli popularnych metod typu get i set. Reszta zmiennych członkowskich jest niewidoczna spoza pliku 
       implementacyjnego, co w pewien sposób ogranicza dostęp do nich.
    - Metody niestanowiące publiczngo interfejsu klasy, zostały zarówno zadeklarowane jak i zdefiniowane w plikach .c,
       co prowadzi do sytuacji podobnej, jak w przypadku wyżej wymienionych "prywatnych" pól.
       Co więcej, zostały one również zadeklarowane jako statyczne, co skutecznie ogranicza zasięg ich wywołań do
       natywnego pliku.

\subsection interfejsy_dziedziczenie Interfejsy, dziedziczenie

Interfejsy pozwalają na rozdzielenie abstrakcji od jej konkretnych, wrażliwych na zmiany i mało elastycznych, implementacji.
Chcąc umożliwić korzystanie z interfejsów (a także miejscami klas abstrakcyjnych), oprócz realizacji ich samych, koniecznym
okazało się wprowadzenie mechanizmu dziedziczenia.
Poniżej wymienione zostały kluczowe cechy zastosowanych w tym względzie rozwiązań:
    1. Jako że język C nie wspiera mechanizmu dziedziczenia, został on zaimplementowany w oparciu o kompozycję (zawieranie)
       obiektów.
    2. Naturalną konsekwencją powyższego rozwiązania jest fakt, iż interfejsy i klasy abstrakcyjne są w istocie reprezentowane
       przez zwykłe klasy (przerobione struktury) i ich instancje, referencje do których zawarte są w klasach dziedziczących.
    3. Każda klasa implementująca interfejs, powinna posiadać zdefiniowaną metodę o sygnaturze:
       
            NazwaKlasy_asNazwaInterfejsu(...) 
       
       zwracającą wskaźnik instancję tegoż, celem umożliwienia rzutowania i polimorficznych wywołań metod.  
    4. Implementacja interfejsu przebiega w sposób następujący:
        - W konstruktorze klasy implementującej tworzona jest instancja interfejsu.
        - Aby utworzyć instancję interfejsu, do jej konstruktora przekazane muszą zostać wskaźniki na implementacje
            deklarowanych przez niego metod oraz, co istotne, wskaźnik na tworzoną instancję klasy dziedziczącej.
        - Przekazanie wartości NULL zamiast poprawnego wskaźnika skutkuje ostrzeżeniem w postaci komunikatu.
    5. Spełnienie wszystkich wymienionych w 4) warunków daje gwarancję, iż każda klasa implementująca pewien interfejs
        może zostać rzutowana na tenże poprzez wywołanie NazwaKlasy_asNazwaInterfejsu(...), a
        uzyskana w ten sposób referencja do instancji interfejsu posiada wszelkie zasoby potrzebne do wykonywania polimorficznych wywołań metod 'wirtualnych'.
    6. Nazwy interfejsów zwyczajowo prefiksowane są wielką literą "I", np. IGameController, IBoard.

\section specyfikacja_zewnetrzna Specyfikacja zewnętrzna

\subsection standardowa_konfiguracja Standardowa konfiguracja

W przypadku standardowym, wszelka konfiguracja przeznaczona dla użytkownika dostępna jest w trakcie działania programu
i ogranicza się do:
    1. Wyboru pliku, w którym zapisana jest plansza.
    2. Wyboru, do ilu pionków pozostałych (dodatkowo) na planszy toczy się gra (a więc poziomu trudności).

\subsection rozszerzanie_funkcjonalnosci Rozszerzanie funkcjonalności

O ile cel gry przyjąć musi jedną z odgórnie ustalonych całkowitych wartości [0, 5], o tyle pliki planszy, dzięki swojej bardzo prostej budowie, mogą być tworzone przez użytkownika wedle uznania.
Dopuszczalny format pliku wejściowego z planszą to: 
    1. Dwie liczby całkowite oddzielone znakiem białym: {liczba wierszy planszy} {liczba kolumn planszy}
    2. Zawartości pól planszy, przedstawione w postaci pojedynczych znaków ASCII o następującej konwencji kodowania:
        - '#' - brak pola tudzież pole nieinteraktywne
        - '_' - pole puste
        - 'o' - pole z żetonem

Przykładowa zawartość pliku planszy:

    7 7
    ##ooo##
    ##ooo##
    ooooooo
    ooo_ooo
    ooooooo
    ##ooo##
    ##ooo##


Zaleca się umieszczanie plików planszy w folderze "data" w strukturze projektu.

\subsection komunikaty_bledow Komunikaty błędów

Komunikaty, które mogą pojawiać się podczas działania programu pozwalają na ogół dość precyzyjnie określić
przyczynę swojego wystąpienia, jednakże dla porządku poniżej wymienione zostały wszystkie alerty, których spodziewać się może
użytkownik aplikacji, zakładając, że działa ona poprawnie:

    1. "Failed to open specified board file."  
    2. "Failed to parse board dimensions."
    3. "An error occurred while creating game board."
    4. "Unrecognized field type character. Defaulting to empty field."

Jak łatwo zauważyć wszystkie związane są z formatem danych wejściowych, a więc jedynym składnikiem aplikacji 
z jakim styczność ma użytkownik końcowy. Ich treść nie wydaje się wymagać dodatkowego komentarza.

Oprócz wyżej wymienionych komunikatów w kodzie źródłowym obecne są także inne, pełniące jednak
rolę wskazówek dla programisty modyfikującego bądź rozszerzającego działanie aplikacji.
Zostały one podzielone na kategorie, zgodnie obszarami aplikacji których dotyczą:

Błędy API kontrolera:

    1. "Error: no ongoing match to continue." 
    2. "Error: invalid jump data." 
    3. "Error: invalid activation data." 
    4. "Error: there are no recorded jumps to extract."
            
Błędy API modelu:

    1. "Error: cannot decrement score below 0!"
    2. "Error: jump history is full. Cannot add another record." 
    3. "Error: there are no recorded jumps to extract."

Błędy widoku:

    1. "Error: Unrecognized field type. Failed to convert to char."

Ogólne (z komentarzem):

    1. "We should never stop here." - Asercja. Wykonany został blok 'default' instrukcji switch, mimo iż wszystkie możliwe przypadki powinny
                                      zostać otwarcie wymienione w instrukcjach 'case'. 
    2. "Error: NULL passed as interface override." - Zamiast oczekiwanego wskaźnika do tworzonej instancji interfejsu przekazano NULL. Patrz - 2.4.4. 
    3. "Warning: Specified observer not found, thus cannot be removed. " - Usuwanie obserwatora nie powiodło się, gdyż nie został on odnaleziony w docelowej tablicy.
    4. "Warning: Maximum number of observers, cannot add another one. " - Dodawania obserwatora nie powiodło się, gdyż przeznaczona na nie tablica została juz wypełniona.        



\section testy Testy

\subsection dane_wejsciowe Dane wejściowe

Częścią programu najbardziej wrażliwą na czynniki zewnętrzne, a więc bezwzględnie wymagającą testowania jest format pliku wejściowego.
W folderze 'tests' drzewa projektu znajdują się wzory plansz, dla których aplikacja została pomyślnie przetestowana.
Pełna lista nazw wyżej wspomnianych plików zaprezentowana jest także poniżej:

    1. dimension_overvalued.board
    2. dimension_undervalued.board
    3. invalid_dimensions.board
    4. lacking_dimension.board
    5. no_dimensions.board
    6. empty_board.board
    7. instant_lose.board
    8. one_token.board
    9. unrecognized_char.board

Zamieszczony został tam także plik o nazwie "win_test.board" umożliwiający osiągnięcie zwycięstwa w pojedynczym ruchu.

\subsection wycieki_pamieci Wycieki pamięci

Autor dołożył wszelkich starań, aby uniknąć wycieków pamięci podczas pracy programu, jednakże ze względu na wybraną architekturę i rozwiązania projektowe
było to zadanie niełatwe.

Po przetestowaniu programu pod tym kątem okazało się, iż ich występowanie jest wysoce prawdopodobne, jednakże jedną z przyczyn tego zjawiska 
może być specyfika biblioteki GTK+. Wykorzystywanie zasobów zewnętrznych takich jak pliki .XML czy .css wydaje się być
przyczyną generowania fałszywych komunikatów o wyciekach pamięci przez program diagnostyczny (co tłumaczyłoby przynajmniej część występujących przypadków).       
Niestety nie udało się stwierdzić jak duża część potencjalnych wycieków jest kwestią czynników zewnętrznych, a jak duża jest konsekwencją działań autora. 

\section wnioski Wnioski

Proces twórczy projektu znacząco pogłębił wiedzę autora o języku C, lecz nie była to jedyna płynąca z niego korzyść.
Wykorzystanie biblioteki GTK+ do stworzenia interfejsu użytkownika oraz sam projekt architektury aplikacji pozwoliły autorowi zdobyć
podstawowe doświadczenie w zakresie tworzenia aplikacji okienkowych, a także odświeżyć znajomość pewnych programistyczych koncepcji,
takich jak wzorzec "Obserwator" czy system sygnałów.
Największym wyzwaniem okazało się samodzielne zarządzanie wielokrotnie dynamicznie alokowaną pamięcią.
Stosunkowo problematyczną była również własnoręczna implementacja charakterystycznych dla programowania obiektowego rozwiązań, 
takich jak dziedziczenie, metody wirtualne czy prywatne zmienne członkowskie.

*/