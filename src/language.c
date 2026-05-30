#include "../include/browseos.h"

static LanguageInfo lang_data[] = {
    {"en", "English", "English", "US", 0, '.', 0, "Segoe UI"},
    {"zh-CN", "Chinese (Simplified)", "\u4e2d\u6587\u7b80\u4f53", "CN", 0, '.', 0, "Microsoft YaHei"},
    {"zh-TW", "Chinese (Traditional)", "\u4e2d\u6587\u7e41\u9ad4", "TW", 0, '.', 0, "Microsoft JhengHei"},
    {"ja", "Japanese", "\u65e5\u672c\u8a9e", "JP", 0, '.', 0, "Yu Gothic"},
    {"ko", "Korean", "\ud55c\uad6d\uc5b4", "KR", 0, '.', 0, "Malgun Gothic"},
    {"fr", "French", "Fran\u00e7ais", "FR", 0, ',', 1, "Segoe UI"},
    {"de", "German", "Deutsch", "DE", 0, ',', 1, "Segoe UI"},
    {"es", "Spanish", "Espa\u00f1ol", "ES", 0, ',', 1, "Segoe UI"},
    {"ru", "Russian", "\u0420\u0443\u0441\u0441\u043a\u0438\u0439", "RU", 0, ',', 0, "Segoe UI"},
    {"ar", "Arabic", "\u0627\u0644\u0639\u0631\u0628\u064a\u0629", "SA", 1, '.', 0, "Segoe UI"},
    {"pt", "Portuguese", "Portugu\u00eas", "BR", 0, ',', 1, "Segoe UI"},
    {"it", "Italian", "Italiano", "IT", 0, ',', 1, "Segoe UI"},
    {"nl", "Dutch", "Nederlands", "NL", 0, ',', 1, "Segoe UI"},
    {"pl", "Polish", "Polski", "PL", 0, ',', 0, "Segoe UI"},
    {"tr", "Turkish", "T\u00fcrk\u00e7e", "TR", 0, ',', 0, "Segoe UI"},
    {"sv", "Swedish", "Svenska", "SE", 0, ',', 1, "Segoe UI"},
    {"da", "Danish", "Dansk", "DK", 0, ',', 1, "Segoe UI"},
    {"fi", "Finnish", "Suomi", "FI", 0, ',', 1, "Segoe UI"},
    {"cs", "Czech", "\u010ce\u0161tina", "CZ", 0, ',', 0, "Segoe UI"},
    {"hu", "Hungarian", "Magyar", "HU", 0, ',', 0, "Segoe UI"},
    {"ro", "Romanian", "Rom\u00e2n\u0103", "RO", 0, ',', 0, "Segoe UI"},
    {"vi", "Vietnamese", "Ti\u1ebfng Vi\u1ec7t", "VN", 0, ',', 1, "Segoe UI"},
    {"th", "Thai", "\u0e44\u0e17\u0e22", "TH", 0, '.', 0, "Leelawadee UI"},
    {"he", "Hebrew", "\u05e2\u05d1\u05e8\u05d9\u05ea", "IL", 1, '.', 0, "Segoe UI"}
};

typedef struct {
    const char *key;
    const char *translations[MAX_LANGUAGES];
} LangEntry;

#define LANG_ENTRY(key, ...) { key, { __VA_ARGS__ } }

static LangEntry lang_entries[] = {
    LANG_ENTRY("banner.title",
        "=== BrowseOS v3.0 - Golden Sakura ===",
        "=== BrowseOS v3.0 - \u91d1\u8272\u6a31\u82b1 ===",
        "=== BrowseOS v3.0 - \u91d1\u8272\u6afb\u82b1 ===",
        "=== BrowseOS v3.0 - \u9ec4\u91d1\u306e\u685c ===",
        "=== BrowseOS v3.0 - \ud669\uae08 \ubc8b\uaf43 ===",
        "=== BrowseOS v3.0 - Sakura Dor\u00e9 ===",
        "=== BrowseOS v3.0 - Goldene Kirsche ===",
        "=== BrowseOS v3.0 - Sakura Dorada ===",
        "=== BrowseOS v3.0 - \u0417\u043e\u043b\u043e\u0442\u0430\u044f \u0421\u0430\u043a\u0443\u0440\u0430 ===",
        "=== BrowseOS v3.0 - \u0627\u0644\u0633\u0627\u0643\u0648\u0631\u0627 \u0627\u0644\u0630\u0647\u0628\u064a\u0629 ===",
        "=== BrowseOS v3.0 - Sakura Dourada ===",
        "=== BrowseOS v3.0 - Sakura Dorata ===",
        "=== BrowseOS v3.0 - Gouden Sakura ===",
        "=== BrowseOS v3.0 - Z\u0142ota Sakura ===",
        "=== BrowseOS v3.0 - Alt\u0131n Sakura ===",
        "=== BrowseOS v3.0 - Gyllene Sakura ===",
        "=== BrowseOS v3.0 - Gylden Sakura ===",
        "=== BrowseOS v3.0 - Kultainen Sakura ===",
        "=== BrowseOS v3.0 - Zlat\u00e1 Sakura ===",
        "=== BrowseOS v3.0 - Arany Sakura ===",
        "=== BrowseOS v3.0 - Sakura de Aur ===",
        "=== BrowseOS v3.0 - Sakura V\u00e0ng ===",
        "=== BrowseOS v3.0 - \u0e0b\u0e32\u0e01\u0e38\u0e23\u0e30\u0e2a\u0e35\u0e17\u0e2d\u0e07 ===",
        "=== BrowseOS v3.0 - \u05e1\u05e7\u05d5\u05e8\u05d4 \u05d4\u05d6\u05d4\u05d5\u05d1 ==="),

    LANG_ENTRY("welcome",
        "Welcome to BrowseOS - The Browser Operating System",
        "\u6b22\u8fce\u4f7f\u7528 BrowseOS - \u6d4f\u89c8\u5668\u64cd\u4f5c\u7cfb\u7edf",
        "\u6b61\u8fce\u4f7f\u7528 BrowseOS - \u700f\u89bd\u5668\u64cd\u4f5c\u7cfb\u7d71",
        "BrowseOS \u3078\u3088\u3046\u3053\u305d - \u30d6\u30e9\u30a6\u30b6\u30aa\u30da\u30ec\u30fc\u30c6\u30a3\u30f3\u30b0\u30b7\u30b9\u30c6\u30e0",
        "BrowseOS\uc5d0 \uc624\uc2e0 \uac83\uc744 \ud658\uc601\ud569\ub2c8\ub2e4 - \ube0c\ub77c\uc6b0\uc800 \uc6b4\uc601\uccb4\uc81c",
        "Bienvenue sur BrowseOS - Le Syst\u00e8me d'Exploitation Navigateur",
        "Willkommen bei BrowseOS - Dem Browser-Betriebssystem",
        "Bienvenido a BrowseOS - El Sistema Operativo Navegador",
        "\u0414\u043e\u0431\u0440\u043e \u043f\u043e\u0436\u0430\u043b\u043e\u0432\u0430\u0442\u044c \u0432 BrowseOS - \u0411\u0440\u0430\u0443\u0437\u0435\u0440\u043d\u0430\u044f \u041e\u0421",
        "\u0645\u0631\u062d\u0628\u064b\u0627 \u0628\u0643 \u0641\u064a BrowseOS - \u0646\u0638\u0627\u0645 \u062a\u0634\u063a\u064a\u0644 \u0627\u0644\u0645\u062a\u0635\u0641\u062d",
        "Bem-vindo ao BrowseOS - O Sistema Operacional Navegador",
        "Benvenuto su BrowseOS - Il Sistema Operativo Browser",
        "Welkom bij BrowseOS - Het Browser Besturingssysteem",
        "Witamy w BrowseOS - Systemie Operacyjnym Przegl\u0105darki",
        "BrowseOS'a Ho\u015f Geldiniz - Taray\u0131c\u0131 \u0130\u015fletim Sistemi",
        "V\u00e4lkommen till BrowseOS - Webbl\u00e4sarens Operativsystem",
        "Velkommen til BrowseOS - Browserstyresystemet",
        "Tervetuloa BrowseOSiin - Selaink\u00e4ytt\u00f6j\u00e4rjestelm\u00e4",
        "V\u00edtejte v BrowseOS - Prohl\u00ed\u017ee\u010dov\u00fd Opera\u010dn\u00ed Syst\u00e9m",
        "\u00dcdv\u00f6z\u00f6lj\u00fck a BrowseOS-ban - A B\u00f6ng\u00e9sz\u0151 Oper\u00e1ci\u00f3s Rendszer",
        "Bine a\u021bi venit \u00een BrowseOS - Sistemul de Operare Browser",
        "Ch\u00e0o m\u1eebng b\u1ea1n \u0111\u1ebfn v\u1edbi BrowseOS - H\u1ec7 \u0110i\u1ec1u H\u00e0nh Tr\u00ecnh Duy\u1ec7t",
        "\u0e22\u0e34\u0e19\u0e14\u0e35\u0e15\u0e49\u0e2d\u0e19\u0e23\u0e31\u0e1a\u0e2a\u0e39\u0e48 BrowseOS - \u0e23\u0e30\u0e1a\u0e1a\u0e1b\u0e0f\u0e34\u0e1a\u0e31\u0e15\u0e34\u0e01\u0e32\u0e23\u0e40\u0e1a\u0e23\u0e32\u0e27\u0e4c\u0e40\u0e0b\u0e2d\u0e23\u0e4c",
        "\u05d1\u05e8\u05d5\u05da \u05d4\u05d1\u05d0 \u05dc-BrowseOS - \u05de\u05e2\u05e8\u05db\u05ea \u05d4\u05e4\u05e2\u05dc\u05d4 \u05d3\u05e4\u05d3\u05e4\u05df"),

    LANG_ENTRY("prompt", "browseos> ", "browseos> ", "browseos> ", "browseos> ", "browseos> ",
        "browseos> ", "browseos> ", "browseos> ", "browseos> ", "browseos> ",
        "browseos> ", "browseos> ", "browseos> ", "browseos> ", "browseos> ",
        "browseos> ", "browseos> ", "browseos> ", "browseos> ", "browseos> ",
        "browseos> ", "browseos> ", "browseos> ", "browseos> "),

    LANG_ENTRY("cmd.notfound", "Command not found: %s. Type 'help' for available commands.",
        "\u627e\u4e0d\u5230\u547d\u4ee4\uff1a%s\u3002\u8f93\u5165 'help' \u67e5\u770b\u53ef\u7528\u547d\u4ee4\u3002",
        "\u627e\u4e0d\u5230\u547d\u4ee4\uff1a%s\u3002\u8f93\u5165 'help' \u67e5\u770b\u53ef\u7528\u547d\u4ee4\u3002",
        "\u30b3\u30de\u30f3\u30c9\u304c\u898b\u3064\u304b\u308a\u307e\u305b\u3093: %s\u3002'help'\u3067\u5229\u7528\u53ef\u80fd\u306a\u30b3\u30de\u30f3\u30c9\u3092\u78ba\u8a8d\u3057\u3066\u304f\u3060\u3055\u3044\u3002",
        "\uba85\ub839\uc744 \ucc3e\uc744 \uc218 \uc5c6\uc2b5\ub2c8\ub2e4: %s. '\ud5ec\ud504'\ub97c \uc785\ub825\ud558\uc5ec \uc0ac\uc6a9 \uac00\ub2a5\ud55c \uba85\ub839\uc744 \ud655\uc778\ud558\uc2ed\uc2dc\uc624.",
        "Commande inconnue: %s. Tapez 'help' pour les commandes disponibles.",
        "Befehl nicht gefunden: %s. Geben Sie 'help' f\u00fcr verf\u00fcgbare Befehle ein.",
        "Comando no encontrado: %s. Escriba 'help' para ver comandos disponibles.",
        "\u041a\u043e\u043c\u0430\u043d\u0434\u0430 \u043d\u0435 \u043d\u0430\u0439\u0434\u0435\u043d\u0430: %s. \u0412\u0432\u0435\u0434\u0438\u0442\u0435 'help' \u0434\u043b\u044f \u0441\u043f\u0438\u0441\u043a\u0430 \u043a\u043e\u043c\u0430\u043d\u0434.",
        "\u0627\u0644\u0623\u0645\u0631 \u063a\u064a\u0631 \u0645\u0648\u062c\u0648\u062f: %s. \u0627\u0643\u062a\u0628 'help' \u0644\u0644\u062d\u0635\u0648\u0644 \u0639\u0644\u0649 \u0627\u0644\u0623\u0648\u0627\u0645\u0631 \u0627\u0644\u0645\u062a\u0627\u062d\u0629.",
        "Comando n\u00e3o encontrado: %s. Digite 'help' para comandos dispon\u00edveis.",
        "Comando non trovato: %s. Digita 'help' per i comandi disponibili.",
        "Commando niet gevonden: %s. Typ 'help' voor beschikbare commando's.",
        "Nie znaleziono polecenia: %s. Wpisz 'help', aby zobaczy\u0107 dost\u0119pne polecenia.",
        "Komut bulunamad\u0131: %s. Kullan\u0131labilir komutlar i\u00e7in 'help' yaz\u0131n.",
        "Kommandot hittades inte: %s. Skriv 'help' f\u00f6r tillg\u00e4ngliga kommandon.",
        "Kommando ikke fundet: %s. Skriv 'help' for tilg\u00e6ngelige kommandoer.",
        "Komentoa ei l\u00f6ytynyt: %s. Kirjoita 'help' n\u00e4hd\u00e4ksesi k\u00e4ytett\u00e4viss\u00e4 olevat komennot.",
        "P\u0159\u00edkaz nenalezen: %s. Pro dostupn\u00e9 p\u0159\u00edkazy zadejte 'help'.",
        "A parancs nem tal\u00e1lhat\u00f3: %s. \u00cdrja be a 'help' parancsot az el\u00e9rhet\u0151 parancsokhoz.",
        "Comanda nu a fost g\u0103sit\u0103: %s. Tasta\u021bi 'help' pentru comenzile disponibile.",
        "Kh\u00f4ng t\u00ecm th\u1ea5y l\u1ec7nh: %s. G\u00f5 'help' \u0111\u1ec3 xem c\u00e1c l\u1ec7nh kh\u1ea3 d\u1ee5ng.",
        "\u0e44\u0e21\u0e48\u0e1e\u0e1a\u0e04\u0e33\u0e2a\u0e31\u0e48\u0e07: %s. \u0e1e\u0e34\u0e21\u0e1e\u0e4c 'help' \u0e40\u0e1e\u0e37\u0e48\u0e2d\u0e14\u0e39\u0e04\u0e33\u0e2a\u0e31\u0e48\u0e07\u0e17\u0e35\u0e48\u0e21\u0e35\u0e2d\u0e22\u0e39\u0e48",
        "\u05d4\u05e4\u05e7\u05d5\u05d3\u05d4 \u05dc\u05d0 \u05e0\u05de\u05e6\u05d0\u05d4: %s. \u05d4\u05e7\u05dc\u05d3 'help' \u05dc\u05e4\u05e7\u05d5\u05d3\u05d5\u05ea \u05d6\u05de\u05d9\u0646\u05d5\u05ea"),

    /* Continue with many more translation entries */
    LANG_ENTRY("page.title", "Page Title", "\u9875\u9762\u6807\u9898", "\u9801\u9762\u6a19\u984c",
        "\u30da\u30fc\u30b8\u30bf\u30a4\u30c8\u30eb", "\ud398\uc774\uc9c0 \uc81c\ubaa9",
        "Titre de la page", "Seitentitel", "T\u00edtulo de la p\u00e1gina",
        "\u0417\u0430\u0433\u043e\u043b\u043e\u0432\u043e\u043a \u0441\u0442\u0440\u0430\u043d\u0438\u0446\u044b",
        "\u0639\u0646\u0648\u0627\u0646 \u0627\u0644\u0635\u0641\u062d\u0629",
        "T\u00edtulo da P\u00e1gina", "Titolo della pagina", "Paginatitel",
        "Tytu\u0142 strony", "Sayfa Ba\u015fl\u0131\u011f\u0131", "Sidtitel",
        "Sidetitel", "Sivun otsikko", "N\u00e1zev str\u00e1nky", "Oldal c\u00edme",
        "Titlul paginii", "Ti\u00eau \u0111\u1ec1 trang", "\u0e0a\u0e37\u0e48\u0e2d\u0e2b\u0e19\u0e49\u0e32",
        "\u05db\u05d5\u05ea\u05e8\u05ea \u05d4\u05e2\u05de\u05d5\u05d3"),

    LANG_ENTRY("page.loading", "Loading...", "\u52a0\u8f7d\u4e2d...", "\u8f09\u5165\u4e2d...",
        "\u8aad\u307f\u8fbc\u307f\u4e2d...", "\ub85c\ub4dc \uc911...",
        "Chargement...", "L\u00e4dt...", "Cargando...",
        "\u0417\u0430\u0433\u0440\u0443\u0437\u043a\u0430...",
        "\u062c\u0627\u0631 \u0627\u0644\u062a\u062d\u0645\u064a\u0644...",
        "Carregando...", "Caricamento...", "Laden...",
        "\u0141adowanie...", "Y\u00fckleniyor...", "Laddar...",
        "Indl\u00e6ser...", "Ladataan...", "Na\u010d\u00edt\u00e1n\u00ed...",
        "Bet\u00f6lt\u00e9s...", "Se \u00eencarc\u0103...", "\u0110ang t\u1ea3i...",
        "\u0e01\u0e33\u0e25\u0e31\u0e07\u0e42\u0e2b\u0e25\u0e14...",
        "\u05d8\u05d5\u05e2\u05df..."),

    LANG_ENTRY("page.notfound", "404 - Page Not Found",
        "404 - \u9875\u9762\u672a\u627e\u5230",
        "404 - \u9801\u9762\u672a\u627e\u5230",
        "404 - \u30da\u30fc\u30b8\u304c\u898b\u3064\u304b\u308a\u307e\u305b\u3093",
        "404 - \ud398\uc774\uc9c0\ub97c \ucc3e\uc744 \uc218 \uc5c6\uc2b5\ub2c8\ub2e4",
        "404 - Page non trouv\u00e9e",
        "404 - Seite nicht gefunden",
        "404 - P\u00e1gina no encontrada",
        "404 - \u0421\u0442\u0440\u0430\u043d\u0438\u0446\u0430 \u043d\u0435 \u043d\u0430\u0439\u0434\u0435\u043d\u0430",
        "404 - \u0627\u0644\u0635\u0641\u062d\u0629 \u063a\u064a\u0631 \u0645\u0648\u062c\u0648\u062f\u0629",
        "404 - P\u00e1gina n\u00e3o encontrada",
        "404 - Pagina non trovata",
        "404 - Pagina niet gevonden",
        "404 - Strona nie znaleziona",
        "404 - Sayfa Bulunamad\u0131",
        "404 - Sidan hittades inte",
        "404 - Siden blev ikke fundet",
        "404 - Sivua ei l\u00f6ydy",
        "404 - Str\u00e1nka nenalezena",
        "404 - Az oldal nem tal\u00e1lhat\u00f3",
        "404 - Pagina nu a fost g\u0103sit\u0103",
        "404 - Kh\u00f4ng t\u00ecm th\u1ea5y trang",
        "404 - \u0e44\u0e21\u0e48\u0e1e\u0e1a\u0e2b\u0e19\u0e49\u0e32",
        "404 - \u05d4\u05e2\u05de\u05d5\u05d3 \u05dc\u05d0 \u05e0\u05de\u05e6\u05d0"),

    LANG_ENTRY("setting.changed", "Setting '%s' changed to '%s'",
        "\u8bbe\u7f6e '%s' \u5df2\u66f4\u6539\u4e3a '%s'",
        "\u8a2d\u5b9a '%s' \u5df2\u66f4\u6539\u70ba '%s'",
        "\u8a2d\u5b9a '%s' \u304c '%s' \u306b\u5909\u66f4\u3055\u308c\u307e\u3057\u305f",
        "\uc124\uc815 '%s'\uc774(\uac00) '%s'(\uc73c)\ub85c \ubcc0\uacbd\ub418\uc5c8\uc2b5\ub2c8\ub2e4",
        "Le param\u00e8tre '%s' a \u00e9t\u00e9 modifi\u00e9 en '%s'",
        "Einstellung '%s' wurde zu '%s' ge\u00e4ndert",
        "El ajuste '%s' se cambi\u00f3 a '%s'",
        "\u041d\u0430\u0441\u0442\u0440\u043e\u0439\u043a\u0430 '%s' \u0438\u0437\u043c\u0435\u043d\u0435\u043d\u0430 \u043d\u0430 '%s'",
        "\u062a\u0645 \u062a\u063a\u064a\u064a\u0631 \u0627\u0644\u0625\u0639\u062f\u0627\u062f '%s' \u0625\u0644\u0649 '%s'",
        "Configura\u00e7\u00e3o '%s' alterada para '%s'",
        "Impostazione '%s' cambiata a '%s'",
        "Instelling '%s' gewijzigd naar '%s'",
        "Ustawienie '%s' zmieniono na '%s'",
        "Ayar '%s', '%s' olarak de\u011fi\u015ftirildi",
        "Inst\u00e4llningen '%s' \u00e4ndrades till '%s'",
        "Indstillingen '%s' blev \u00e6ndret til '%s'",
        "Asetus '%s' muutettu arvoon '%s'",
        "Nastaven\u00ed '%s' zm\u011bn\u011bno na '%s'",
        "A '%s' be\u00e1ll\u00edt\u00e1s megv\u00e1ltozott erre: '%s'",
        "Setarea '%s' a fost schimbat\u0103 \u00een '%s'",
        "C\u00e0i \u0111\u1eb7t '%s' \u0111\u00e3 \u0111\u01b0\u1ee3c thay \u0111\u1ed5i th\u00e0nh '%s'",
        "\u0e01\u0e32\u0e23\u0e15\u0e31\u0e49\u0e07\u0e04\u0e48\u0e32 '%s' \u0e40\u0e1b\u0e25\u0e35\u0e48\u0e22\u0e19\u0e40\u0e1b\u0e47\u0e19 '%s'",
        "\u05d4\u05d4\u05d2\u05d3\u05e8\u05d4 '%s' \u05e9\u05d5\u05e0\u05ea\u05d4 \u05dc-'%s'"),

    LANG_ENTRY("engine.switched", "Switched to %s engine (v%s)",
        "\u5df2\u5207\u6362\u5230 %s \u5f15\u64ce\uff08v%s\uff09",
        "\u5df2\u5207\u63db\u5230 %s \u5f15\u64ce\uff08v%s\uff09",
        "%s \u30a8\u30f3\u30b8\u30f3\u306b\u5207\u308a\u66ff\u3048\u307e\u3057\u305f (v%s)",
        "%s \uc5d4\uc9c4\uc73c\ub85c \uc804\ud658\ud588\uc2b5\ub2c8\ub2e4 (v%s)",
        "Moteur %s activ\u00e9 (v%s)",
        "Zu %s Engine gewechselt (v%s)",
        "Motor %s activado (v%s)",
        "\u041f\u0435\u0440\u0435\u043a\u043b\u044e\u0447\u0435\u043d\u043e \u043d\u0430 \u0434\u0432\u0438\u0436\u043e\u043a %s (v%s)",
        "\u062a\u0645 \u0627\u0644\u062a\u0628\u062f\u064a\u0644 \u0625\u0644\u0649 \u0645\u062d\u0631\u0643 %s (\u0625\u0635\u062f\u0627\u0631 %s)",
        "Motor %s ativado (v%s)",
        "Motore %s attivato (v%s)",
        "Overgeschakeld naar %s engine (v%s)",
        "Prze\u0142\u0105czono na silnik %s (v%s)",
        "%s motoruna ge\u00e7ildi (v%s)",
        "V\u00e4xlade till %s motor (v%s)",
        "Skiftede til %s-motor (v%s)",
        "Vaihdettiin %s-moottoriin (v%s)",
        "P\u0159epnuto na %s engine (v%s)",
        "V\u00e1lt\u00e1s %s motorra (v%s)",
        "S-a comutat la motorul %s (v%s)",
        "\u0110\u00e3 chuy\u1ec3n sang c\u00f4ng c\u1ed5 %s (v%s)",
        "\u0e40\u0e1b\u0e25\u0e35\u0e48\u0e22\u0e19\u0e40\u0e1b\u0e47\u0e19\u0e40\u0e04\u0e23\u0e37\u0e48\u0e2d\u0e07\u0e21\u0e37\u0e2d %s (v%s)",
        "\u05d4\u05d5\u05de\u05ea\u05dc \u05dc\u05de\u05e0\u05d5\u05e2 %s (\u05d2\u05e8\u05e1\u05d4 %s)"),

    LANG_ENTRY("lang.switched", "Switched to %s (%s)",
        "\u5df2\u5207\u6362\u5230 %s (%s)",
        "\u5df2\u5207\u63db\u5230 %s (%s)",
        "%s (%s) \u306b\u5207\u308a\u66ff\u3048\u307e\u3057\u305f",
        "%s(%s)(\u73b0)\u65e5\u672c\u8a9e\u306b\u5207\u308a\u66ff\u3048\u307e\u3057\u305f",
        "\u062a\u0645 \u0627\u0644\u062a\u0628\u062f\u064a\u0644 \u0625\u0644\u0649 %s (%s)",
        "Langue chang\u00e9e en %s (%s)",
        "Sprache gewechselt zu %s (%s)",
        "Idioma cambiado a %s (%s)",
        "\u042f\u0437\u044b\u043a \u0438\u0437\u043c\u0435\u043d\u0435\u043d \u043d\u0430 %s (%s)",
        "\u062a\u0645 \u062a\u0628\u062f\u064a\u0644 \u0627\u0644\u0644\u063a\u0629 \u0625\u0644\u0649 %s (%s)",
        "Idioma alterado para %s (%s)",
        "Lingua cambiata a %s (%s)",
        "Taal gewijzigd naar %s (%s)",
        "J\u0119zyk zmieniony na %s (%s)",
        "Dil %s (%s) olarak de\u011fi\u015ftirildi",
        "Spr\u00e5k \u00e4ndrat till %s (%s)",
        "Sprog skiftet til %s (%s)",
        "Kieli vaihdettu: %s (%s)",
        "Jazyk zm\u011bn\u011bn na %s (%s)",
        "Nyelv v\u00e1lt\u00e1s: %s (%s)",
        "Limba a fost schimbat\u0103 \u00een %s (%s)",
        "\u0110\u00e3 chuy\u1ec3n sang ng\u00f4n ng\u1eef %s (%s)",
        "\u0e40\u0e1b\u0e25\u0e35\u0e48\u0e22\u0e19\u0e20\u0e32\u0e29\u0e32\u0e40\u0e1b\u0e47\u0e19 %s (%s)",
        "\u05d4\u05e9\u05e4\u05d4 \u05d4\u05d5\u05de\u05d7\u05dc\u05e4\u05d4 \u05dc-%s (%s)"),

    LANG_ENTRY("search.prompt", "Search: ", "\u641c\u7d22: ", "\u641c\u5c0b: ",
        "\u691c\u7d22: ", "\uac80\uc0c9: ",
        "Rechercher: ", "Suche: ", "Buscar: ",
        "\u041f\u043e\u0438\u0441\u043a: ",
        "\u0628\u062d\u062b: ", "Pesquisar: ", "Cerca: ",
        "Zoeken: ", "Szukaj: ", "Ara: ", "S\u00f6k: ",
        "S\u00f8g: ", "Haku: ", "Hledat: ", "Keres\u00e9s: ",
        "C\u0103utare: ", "T\u00ecm ki\u1ebfm: ",
        "\u0e04\u0e49\u0e19\u0e2b\u0e32: ", "\u05d7\u05d9\u05e4\u05d5\u05e9: "),

    LANG_ENTRY("bookmark.added", "Bookmark added: %s",
        "\u4e66\u7b7e\u5df2\u6dfb\u52a0: %s",
        "\u66f8\u7c64\u5df2\u65b0\u589e: %s",
        "\u30d6\u30c3\u30af\u30de\u30fc\u30af\u306b\u8ffd\u52a0\u3057\u307e\u3057\u305f: %s",
        "\ubd81\ub9c8\ud06c\uac00 \ucd94\uac00\ub418\uc5c8\uc2b5\ub2c8\ub2e4: %s",
        "Marque-page ajout\u00e9: %s",
        "Lesezeichen hinzugef\u00fcgt: %s",
        "Marcador a\u00f1adido: %s",
        "\u0417\u0430\u043a\u043b\u0430\u0434\u043a\u0430 \u0434\u043e\u0431\u0430\u0432\u043b\u0435\u043d\u0430: %s",
        "\u062a\u0645\u062a \u0625\u0636\u0627\u0641\u0629 \u0627\u0644\u0639\u0644\u0627\u0645\u0629: %s",
        "Favorito adicionado: %s",
        "Segnalibro aggiunto: %s",
        "Bladwijzer toegevoegd: %s",
        "Zak\u0142adka dodana: %s",
        "Yer imi eklendi: %s",
        "Bokm\u00e4rke tillagt: %s",
        "Bogm\u00e6rke tilf\u00f8jet: %s",
        "Kirjanmerkki lis\u00e4tty: %s",
        "Z\u00e1lo\u017eka p\u0159id\u00e1na: %s",
        "K\u00f6nyvjelz\u0151 hozz\u00e1adva: %s",
        "Semn de carte ad\u0103ugat: %s",
        "\u0110\u00e3 th\u00eam d\u1ea5u trang: %s",
        "\u0e40\u0e1e\u0e34\u0e48\u0e21\u0e44\u0e21\u0e49\u0e04\u0e4c\u0e1a\u0e38\u0e4a\u0e04\u0e21\u0e32\u0e23\u0e4c\u0e04\u0e41\u0e25\u0e49\u0e27: %s",
        "\u05d4\u05e1\u05d9\u05de\u05e0\u05d9\u05d4 \u05e0\u05d5\u05e1\u05e4\u05d4: %s"),

    LANG_ENTRY("download.complete", "Download complete: %s (%d MB)",
        "\u4e0b\u8f7d\u5b8c\u6210: %s (%d MB)",
        "\u4e0b\u8f09\u5b8c\u6210: %s (%d MB)",
        "\u30c0\u30a6\u30f3\u30ed\u30fc\u30c9\u5b8c\u4e86: %s (%d MB)",
        "\ub2e4\uc6b4\ub85c\ub4dc \uc644\ub8cc: %s (%d MB)",
        "T\u00e9l\u00e9chargement termin\u00e9: %s (%d MB)",
        "Download abgeschlossen: %s (%d MB)",
        "Descarga completa: %s (%d MB)",
        "\u0417\u0430\u0433\u0440\u0443\u0437\u043a\u0430 \u0437\u0430\u0432\u0435\u0440\u0448\u0435\u043d\u0430: %s (%d \u041c\u0411)",
        "\u0627\u0643\u062a\u0645\u0644 \u0627\u0644\u062a\u0646\u0632\u064a\u0644: %s (%d MB)",
        "Download conclu\u00eddo: %s (%d MB)",
        "Download completato: %s (%d MB)",
        "Download voltooid: %s (%d MB)",
        "Pobieranie zako\u0144czone: %s (%d MB)",
        "\u0130ndirme tamamland\u0131: %s (%d MB)",
        "Nedladdning klar: %s (%d MB)",
        "Download fuldf\u00f8rt: %s (%d MB)",
        "Lataus valmis: %s (%d MB)",
        "Stahov\u00e1n\u00ed dokon\u010deno: %s (%d MB)",
        "Let\u00f6lt\u00e9s k\u00e9sz: %s (%d MB)",
        "Desc\u0103rcare complet\u0103: %s (%d MB)",
        "T\u1ea3i xu\u1ed1ng ho\u00e0n t\u1ea5t: %s (%d MB)",
        "\u0e14\u0e32\u0e27\u0e19\u0e4c\u0e42\u0e2b\u0e25\u0e14\u0e2a\u0e21\u0e1a\u0e39\u0e23\u0e13\u0e4c: %s (%d MB)",
        "\u05d4\u05d5\u05e8\u05d3\u05d4 \u05d4\u05e9\u05dc\u05de\u05d4: %s (%d MB)"),
};

#define NUM_LANG_ENTRIES (sizeof(lang_entries) / sizeof(lang_entries[0]))

void lang_init_all(BrowserOS *os) {
    int num_langs = sizeof(lang_data) / sizeof(lang_data[0]);
    int i;
    for (i = 0; i < num_langs && i < MAX_LANGUAGES; i++) {
        os->languages[i] = lang_data[i];
    }
    os->num_languages = (num_langs < MAX_LANGUAGES) ? num_langs : MAX_LANGUAGES;
    os->active_language = LANG_EN;
}

int lang_switch(BrowserOS *os, Language lang) {
    if (lang < 0 || lang >= os->num_languages) return 0;
    os->active_language = lang;
    return 1;
}

const char* lang_get(BrowserOS *os, const char *key) {
    int i, j;
    for (i = 0; i < NUM_LANG_ENTRIES; i++) {
        if (strcmp(lang_entries[i].key, key) == 0) {
            int lang_idx = os->active_language;
            if (lang_idx >= 0 && lang_idx < MAX_LANGUAGES && lang_entries[i].translations[lang_idx]) {
                return lang_entries[i].translations[lang_idx];
            }
            return lang_entries[i].translations[LANG_EN];
        }
    }
    return key;
}

const char* lang_tr(BrowserOS *os, const char *key, ...) {
    static char buf[MAX_LANG_STR];
    const char *fmt = lang_get(os, key);
    va_list args;
    va_start(args, key);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    return buf;
}

void lang_print_all(BrowserOS *os) {
    int i;
    printf("\n  %-3s %-8s %-24s %-24s %-4s %s\n", "#", "Code", "Language", "Native Name", "RTL", "Flag");
    printf("  %s\n", "--------------------------------------------------------------------");
    for (i = 0; i < os->num_languages; i++) {
        printf("  %-3d %-8s %-24s %-24s %-4s %s%s\n",
               i + 1,
               os->languages[i].code,
               os->languages[i].name,
               os->languages[i].native_name,
               os->languages[i].rtl ? "Yes" : "No",
               os->languages[i].flag,
               (i == os->active_language) ? " *ACTIVE*" : "");
    }
    printf("\n");
}

const char* lang_code(Language l) {
    if (l >= 0 && l < sizeof(lang_data)/sizeof(lang_data[0]))
        return lang_data[l].code;
    return "en";
}

Language lang_from_code(const char *code) {
    int i, n = sizeof(lang_data) / sizeof(lang_data[0]);
    for (i = 0; i < n; i++) {
        if (strcmp(lang_data[i].code, code) == 0)
            return (Language)i;
    }
    return LANG_EN;
}

const char* lang_name(BrowserOS *os) {
    return os->languages[os->active_language].name;
}

int lang_is_rtl(BrowserOS *os) {
    return os->languages[os->active_language].rtl;
}
