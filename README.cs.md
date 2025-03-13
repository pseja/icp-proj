# Nástroj pro vizuální editaci, generování kódu a monitorování běhu interpretovaných konečných automatů

## Interpretovaný konečný automat
Interpretovaný konečný automat zde chápeme jako [automat Mooreova typu](https://cs.wikipedia.org/wiki/Moore%C5%AFv_stroj), který reaguje na vstupní události, provádí vnitřní akce a generuje výstupy.

### Vlastnosti:
- Automat je doplněný o sadu proměnných, nad kterými mohou operovat algoritmy (akce) přiřazené stavům. Automat může reagovat na vstupní události díky podmíněným přechodům a generovat výstupní události v rámci akcí přiřazených stavům. Podmínky přechodů (strážní podmínky) mohou kromě požadovaných vstupních událostí obsahovat i booleovské výrazy nad obsahem proměnných. Kromě toho mohou přechody mít definované zpoždění.
- Díky plánování zpožděných přechodů může takto specifikovaný systém kromě reakcí na vstupy vyvíjet i vlastní aktivitu (proaktivní chování).
- Popsaný model je speciální modifikací stavového diagramu [UML](https://cs.wikipedia.org/wiki/Unified_Modeling_Language), resp. zjednodušenou variantu Execution Control Chart (ECC), používaného v řízení (podle [IEC 61499](https://en.wikipedia.org/wiki/IEC_61499)).
- Pro jednoduchost předpokládáme, že vstupy a výstupy jsou pouze typu `String`. Interní proměnné mohu být libovolného typu, který dovolí inskripční jazyk (standardně C/C++).
- Umístění nějaké hodnoty do některého vstupu je vstupní událost. Na vstupní událost může reagovat některý přechod z aktuálního stavu. Pokud ne, událost se ignoruje. Hodnota, která byla na vstup uložena (tzv. poslední známá hodnota příslušného vstupu) je vždy k dispozici pro čtení a je použitelná jak ve strážích přechodů, tak v akcích stavů.
- Výstupní události se generují odesláním nějaké hodnoty do některého výstupu v rámci akce stavu.
- Příklad a detaily chování automatu budou popsány dále.

## Nástroj pro vizuální editaci, generování kódu a monitorování
### Funkce:
- Umožní vizuálně specifikovat automat (diagram stavů a přechodů, vstupy, výstupy a interní proměnné). Podmínky přechodů a akce stavů lze specifikovat vhodným inskripčním jazykem. ideálně podmnožinou C nebo C++, což usnadní generování kódu (k tomuto ale lze přistoupit i jinak, viz dále). Stejným způsobem budou specifikovány interní proměnné a jejich inicializace.
- Umožní kompletní specifikaci automatu uložit do souboru v čitelné textové podobě, kterou lze případně i ručně editovat. Takto uložený automat lze načíst a dále editovat. Z této textové podoby bude i generován kód (detaily viz dále).
- Umožní jedním kliknutím vygenerovat kód interpretu, přeložit ho, spustit a následně se k němu připojit např. přes UDP sockety a kdykoli injektovat do interpretu vstupy a průběžně přijímat, logovat a v GUI zobrazovat veškeré události v reálném čase. (Spuštění a komunikace ale může být řešena i jinak - např. sdilenou knihovnou, případně interpret nepřekládat, ale použít vestavěný interpret vhodného jazyka.)
- Za běhu zobrazuje aktuální stav (zvýrazněním v diagramu), poslední známé hodnoty všech vstupů, výstupů a interních proměnných.
- Za běhu zobrazuje log událostí: datum a čas, aktuální stav, hodnoty vstupů, výstupů a interních proměnných.
- Za běhu umožní asynchronně (tj. kdykoli za běhu) injektovat aktualizované hodnoty vstupů, zadané uživatelem.
- Při startu se pokusí připojit k běžícímu atutomatu, pokud takový najde. Zjistí si jeho jméno, načte odpovídající zdrojový soubor, zobrazí automat a začne ho monitorovat, tj. průběžně zobrazovat kompletní stav. Běžící automat může na pokyn uzivatele požádat o ukončení.

## Algoritmus provádění interpretovaného časovaného automatu
1. Dokud není požadavek na ukončení:
    - Dokud existuje na vstupu nezávislý proveditelný přechod z aktuálního stavu:
        - Jde-li o okamžitý přechod, změň stav a proveď akci stavu.
        - Jde-li o zpožděný přechod, nastav časovač pro tento přechod.
    - Odstraň časovače přechodů z neaktuálních stavů.
    - Pasivně čekej na časovač nebo externí vstup, pak:
        - Jde-li o timeout časovače, zjisti, který to byl, odstraň ho a proveď odpovídající přechod, tj. změň stav a proveď akci stavu.
        - Jde-li o externí vstup obsahující vstupní událost, najdi proveditelný přechod z aktuálního stavu, který je podmíněný touto událostí.
            - Jde-li o okamžitý přechod, změň stav a proveď akci stavu.
            - Jde-li o zpožděný přechod, nastav časovač pro tento přechod.
        - Jde-li o externí vstup obsahující příkaz pro ukončení, nastav požadavek na ukončení na TRUE.

## Příklady automatů
### Automat TOF5s
- **Jméno**: TOF5s
- **Komentář**: Timer to off, jednoduchá verze.
- **Vstupy**: `in`
- **Výstupy**: `out`
- **Proměnné**: `int timeout = 5000`
- **Stavy a akce**:
    - `IDLE`: `{ output("out", 0) }`
    - `ACTIVE`: `{ output("out", 1) }`
    - `TIMING`: `{ }`
- **Přechody**:
    - `IDLE` → `ACTIVE`: `in [ atoi(valueof("in")) == 1 ]`
    - `ACTIVE` → `TIMING`: `in [ atoi(valueof("in")) == 0 ]`
    - `TIMING` → `ACTIVE`: `in [ atoi(valueof("in")) == 1 ]`
    - `TIMING` → `IDLE`: `@ timeout`

- Pozn.: Předpokládáme, že funkce `output()`, která generuje výstupní událost, automaticky konvertuje jakýkoli datový typ na string.
- Pozn.: Podmínka přechodu se obecně skládá ze 3 částí, přičemž každá může chybět, a uvedli jsme ji ve tvaru: `input_event_name [ bool_expr_in_C_C++ ] @ delay_in_ms`

### Automat TOF
- **Jméno**: TOF
- **Komentář**: Timer to off, umí nastavit timeout a na požádání sdělit zbývající čas timeru.
- **Vstupy**: `in`, `set_to`, `req_rt`
- **Výstupy**: `out`, `rt`
- **Proměnné**: `int timeout = 5000`
- **Stavy a akce**:
    - `IDLE` : `{ if {defined("set_to")} { timeout = atoi(valueof("set_to"))}; output("out", 0); output("rt", 0); }`
    - `ACTIVE`: `{ if {defined("set_to")} { timeout = atoi(valueof("set_to"))}; output("out", 1); output("rt", timeout); }`
    - `TIMING`: `{ if {defined("set_to")} { timeout = atoi(valueof("set_to"))}; output("rt", timeout - elapsed() ); }`
- **Přechody**:
    - `IDLE` → `ACTIVE`: `in [ atoi(vlaueof("in")) == 1 ]`
    - `ACTIVE` → `TIMING`: `in [ atoi(vlaueof("in")) == 0 ]`
    - `TIMING` → `ACTIVE`: `in [ atoi(vlaueof("in")) == 1 ]`
    - `TIMING` → `IDLE`: `@ timeout`
    - `IDLE` → `IDLE`: `set_to`
    - `ACTIVE` → `ACTIVE`: `set_to`
    - `TIMING` → `TIMING`: `set_to`
    - `IDLE` → `IDLE`: `req_rt`
    - `ACTIVE` → `ACTIVE`: `req_rt`
    - `TIMING` → `TIMING`: `req_rt`

- Pozn.: `elapsed()` vrací dobu trvání aktuálního stavu. Pokud přechod nemění stav, doba aktivace se neresetuje.

## Možná rozšíření pro čtyřčlenný tým
- Pracujte s množinou automatů a nechte je komunikovat vhodným způsobem, např. přes [MQTT](https://en.wikipedia.org/wiki/MQTT).
- Zadání lze zobecnit pro interpretovanou [Petriho síť](https://cs.wikipedia.org/wiki/Petriho_s%C3%AD%C5%A5) (obdoba SFC, Grafcet) - toto je doporučeno konzultovat.
- Automaty použijte na atomické úrovni hierarchického blokového schématu po vzoru DEVS (Ziegler) - toto je doporučeno konzultovat.

## Pokud vás zadání projektu zaujalo
- Kontaktujte doc. Janouška ohledně možnosti řešit podobné nebo související téma v rámci BP.

## Obecné pokyny k realizaci projektu
- Zadání definuje podstatné vlastnosti aplikace, které musí být splněny. Předpokládá se, že detaily řešení si doplní řešitelské týmy.
- Při návrhu postupujte tak, aby výsledný program byl dobře použitelný. Hodnotit se bude nejen to, zda splňuje výše uvedené požadavky, ale také JAK je splňuje.
- Návrh musí oddělit vlastní model aplikace od GUI (navrhněte vhodné rozhraní).
- Návrh programu zdokumentujte ve zdrojovém kódu ve formátu pro program `doxygen`.
- Používejte vhodné návrhové vzory ([Design Patterns](https://cs.wikipedia.org/wiki/N%C3%A1vrhov%C3%BD_vzor)) - viz literatura.
- Pro vytváření menu, dialogů a dalších komponent grafického uživatelského rozhraní (GUI) použijte toolkit `Qt` minimálně ve verzi `5.5` (nainstalováno v učebnách a na serveru `merlin` v adresáři `/usr/local/share/Qt-5.5.1/`). Pokud pro vývoj použijete verzi novější/starší než je na učebnách, předpokládáme, že zajistíte zpětnou/dopřednou kompatibilitu vašich programů. (V případě problémů, počítejte s nutností předvedení překladu/sestavení/běhu aplikace na vašem notebooku. Toto se nedoporučuje.)
- Protože toolkity jsou přenositelné na více platforem, měly by vaše programy fungovat minimálně na Linuxu i na Windows. (Nejen Java umožňuje psaní přenositelných aplikací.) Je jedno na které platformě proběhne vývoj aplikací, jen dbejte na pravidla psaní přenositelných programů. (Testovat se bude na Linuxu = server merlin, ale Makefile nesmí být závislý na konkrétním adresáři s lokální instalací toolkitu.)
- Na vytvoření GUI dialogů a menu můžete použít interaktivní editory/generátory.
- Použijte C++17. ([Boost](https://www.boost.org/) raději nepoužívejte. Pokud použijete některé knihovny z Boost, tak použité soubory přibalte a zkontrolujte si, zda se to vše vejde do max. velikosti odevzdaného archivu. Existuje nástroj [BCP](https://github.com/boostorg/bcp) pro výběr podmnožiny Boost.).
- Programy budou vytvořeny zadáním příkazu `make` v adresáři s rozbalenými zdrojovými kódy projektu (Pozor - bude se automaticky překládat). Musíte vytvořit soubor `Makefile`, který nesmí obsahovat absolutní cesty k souborům.
- Výsledný program vhodně pojmenujte. Pro testovací účely půjde vše spustit použitím `make run`.
- Součástí projektu bude programová dokumentace vytvořená programem [Doxygen](https://en.wikipedia.org/wiki/Doxygen) (konfigurační soubor nastavte na HTML výstup se zahrnutím zdrojových textů: `SOURCE_BROWSER = YES`). Dokumentace se vygeneruje automaticky (= nezabalovat do archivu) při zadání `make doxygen` a bude uložena v podadresáři `doc`.
- Součástí projektu bude také konceptuální návrh v `pdf`, obsahující minimálně diagram tříd bez zbytečných implementačních detailů, případně další UML diagramy dokumentující návrh. Očekává se, že implementace bude v souladu s konceptuálním návrhem.

## Poznámky k odevzdávání
- Termín odevzdání je uveden v IS.
- Řešení zabalené ve formátu `.zip` (nebo `.tar.gz`) odevzdá **pouze vedoucí týmu** do IS a to s dostatečným předstihem před uzávěrkou. (Nikdo nebude psát, že neodevzdal, protože *** a posílat to poštou.) Jméno odevzdaného souboru musí mít tvar:
```sh
xname01-xname02.zip
```
nebo
```sh
xname01-xname02-xname03.zip
```

- `xname01` je jméno vedoucího (toho, kdo projekt odevzdal), `xname02`, resp. `xname03` je druhý, resp.třetí člen týmu (ti nic neodevzdávají).
- **POZOR:** Velikost archivu musí odpovídat tomu, co dovolí IS. Proto si raději včas ověřte odevzdatelnost, ať nedojde k problému na poslední chvíli.
- Odevzdaný archiv obsahuje minimálně tuto adresářovou strukturu:
```sh
./src/       # Zdrojové texty
./examples/  # Příklady pro testování
./doc/       # Dokumentace (generovaná)
./README.txt # Základní informace
./Makefile   # Sestavení a spouštění
```
- Neodevzdávejte nic, co lze vytvořit pomocí `make` (spustitelné programy ani dokumentaci, kterou lze vygenerovat použitím `make doxygen`).
    - Poznámka: Příkaz `make clean` všechny produkty překladu smaže, `make pack` vytvoří archiv pro odevzdání - zkontrolujte, zda obsahuje vše potřebné rozbalením v prázdném adresáři a překladem/spuštěním.
- Všechny zdrojové soubory musí obsahovat na začátku komentář se jmény autorů daného modulu, popisem obsahu modulu, atd. Z komentářů by také mělo být patrné, které části byly napsány ručně a které vygenerovány automaticky.
- Pokud některý modul vychází z převzatého kódu (např. z nějakého tutorialu nebo veřejně dostupného opensource projektu), musí být toto explicitně specifikováno v záhlaví modulu a také zmíněno v `README`.
- V dokumentaci (`README`) musí být zřetelně specifikováno, jaká funkcionalita byla implementována kompletně, jaká s nějakými omezeními a jaká vůbec (aby se hodnotitel zbytečně nesnažil zprovoznit něco, o čem je dopředu známo, že je nefunkční). Nesrovnalost mezi dokumentací a realitou bude předmětem bodové penalizace.

## Poznámky k obhajobě projektu
- Projekt musí být demonstrován a obhájen. Na termín obhajoby je nutné se přihlásit v IS. 

## Poznámky k hodnocení
- Projekt je hodnocen na stupnici 0-100 procent/bodů.
- Po ohodnocení může dojít k přepočtu bodů na jednotlivé členy týmu podle návrhu vedoucího týmu, který je zakódován do jména odevzdaného souboru (formát jména pak je:
```sh
xname01-xname02-AA-BB.zip
```
- Součet AA+BB=100, **POZOR:** pro rovnoměrné rozdělení AA=BB=50% není třeba uvádět `-50-50` a je to preferovaná možnost).
- Hodnocení zahrnuje kvalitu OO návrhu, kvalitu implementace v C++, dodržování stylu psaní programu (odsazování, kvalita komentářů, vhodné identifikátory) a především funkčnost programu. Pokud budou ve zdrojových textech chybět podstatné komentáře (zvláště jména autorů), bude projekt hodnocen 0b.
- **POZOR:**
    - Pokud nepůjde přeložit příkazem `make` = 0b nebo předvedete na svém notebooku a vysvětlíte kde byl problém,
    - pokud nebudou komentáře se jmény autorů = 0b.
    - Zcela nevhodně formátovaný a nečitelný kód = 0b.
    - Plagiáty atd. = 0b + návštěva u disciplinární komise
