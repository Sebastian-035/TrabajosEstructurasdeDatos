#include "arbol_gestor.h"
#include <fstream>  // Added for file I/O

// ==================== IMPLEMENTACIONES NODO (de nodo_trie.h) ====================
Nodo::Nodo(int id, const std::string& nombre, TipoNodo tipo,
           const std::string& contenido, Nodo* padre)
    : id(id), nombre(nombre), tipo(tipo),
      contenido(contenido), padre(padre) {

    if (nombre.empty()) {
        throw std::invalid_argument("El nombre del nodo no puede estar vacío");
    }
}

Nodo::~Nodo() {
    hijos.clear();
}

int Nodo::getId() const { return id; }

const std::string& Nodo::getNombre() const { return nombre; }

TipoNodo Nodo::getTipo() const { return tipo; }

const std::string& Nodo::getContenido() const { return contenido; }

Nodo* Nodo::getPadre() const { return padre; }

const std::vector<Nodo*>& Nodo::getHijos() const { return hijos; }

int Nodo::getCantidadHijos() const { return static_cast<int>(hijos.size()); }

std::string Nodo::getRutaCompleta() const {
    if (esRaiz()) {
        return "/";
    }

    std::string ruta;
    construirRutaRecursiva(ruta);
    return ruta;
}

void Nodo::construirRutaRecursiva(std::string& ruta) const {
    if (padre && !padre->esRaiz()) {
        padre->construirRutaRecursiva(ruta);
    }

    if (!ruta.empty() && ruta.back() != '/') {
        ruta += "/";
    }

    ruta += nombre;

    if (esRaiz() || (padre && padre->esRaiz())) {
        ruta = "/" + ruta;
    }
}

std::string Nodo::getTipoString() const {
    return (tipo == CARPETA) ? "CARPETA" : "ARCHIVO";
}

void Nodo::setNombre(const std::string& nuevoNombre) {
    if (nuevoNombre.empty()) {
        throw std::invalid_argument("El nombre no puede estar vacío");
    }
    nombre = nuevoNombre;
}

void Nodo::setContenido(const std::string& nuevoContenido) {
    if (tipo == CARPETA) {
        throw std::logic_error("Una carpeta no puede tener contenido");
    }
    contenido = nuevoContenido;
}

void Nodo::setPadre(Nodo* nuevoPadre) {
    if (nuevoPadre == this) {
        throw std::invalid_argument("Un nodo no puede ser padre de sí mismo");
    }

    Nodo* temp = nuevoPadre;
    while (temp != nullptr) {
        if (temp == this) {
            throw std::invalid_argument("Ciclo detectado: no se puede establecer un descendiente como padre");
        }
        temp = temp->getPadre();
    }

    padre = nuevoPadre;
}

void Nodo::agregarHijo(Nodo* hijo) {
    if (hijo == nullptr) {
        throw std::invalid_argument("No se puede agregar un hijo nulo");
    }

    if (hijo == this) {
        throw std::invalid_argument("Un nodo no puede ser hijo de sí mismo");
    }

    for (const auto& h : hijos) {
        if (h->getNombre() == hijo->getNombre()) {
            throw std::runtime_error("Ya existe un hijo con el nombre: " + hijo->getNombre());
        }
    }

    Nodo* temp = this;
    while (temp != nullptr) {
        if (temp == hijo) {
            throw std::invalid_argument("Ciclo detectado: no se puede agregar un ancestro como hijo");
        }
        temp = temp->getPadre();
    }

    hijos.push_back(hijo);
    hijo->setPadre(this);
}

bool Nodo::eliminarHijo(Nodo* hijo) {
    if (hijo == nullptr) return false;

    auto it = std::find(hijos.begin(), hijos.end(), hijo);
    if (it != hijos.end()) {
        (*it)->setPadre(nullptr);
        hijos.erase(it);
        return true;
    }
    return false;
}

bool Nodo::eliminarHijoPorId(int id) {
    auto it = std::find_if(hijos.begin(), hijos.end(),
        [id](Nodo* n) { return n->getId() == id; });

    if (it != hijos.end()) {
        (*it)->setPadre(nullptr);
        hijos.erase(it);
        return true;
    }
    return false;
}

bool Nodo::eliminarHijoPorNombre(const std::string& nombre) {
    auto it = std::find_if(hijos.begin(), hijos.end(),
        [&nombre](Nodo* n) { return n->getNombre() == nombre; });

    if (it != hijos.end()) {
        (*it)->setPadre(nullptr);
        hijos.erase(it);
        return true;
    }
    return false;
}

Nodo* Nodo::buscarHijoPorId(int id) const {
    auto it = std::find_if(hijos.begin(), hijos.end(),
        [id](Nodo* n) { return n->getId() == id; });

    return (it != hijos.end()) ? *it : nullptr;
}

Nodo* Nodo::buscarHijoPorNombre(const std::string& nombre) const {
    auto it = std::find_if(hijos.begin(), hijos.end(),
        [&nombre](Nodo* n) { return n->getNombre() == nombre; });

    return (it != hijos.end()) ? *it : nullptr;
}

bool Nodo::esRaiz() const { return (padre == nullptr); }

bool Nodo::tieneHijos() const { return !hijos.empty(); }

void Nodo::limpiarHijos() {
    for (auto& hijo : hijos) {
        hijo->setPadre(nullptr);
    }
    hijos.clear();
}

// ==================== IMPLEMENTACIONES TRIE (de nodo_trie.h) ====================
TrieNode::TrieNode() : esFinal(false) {}

Trie::Trie() : raiz(std::make_shared<TrieNode>()) {}

void Trie::insertar(const std::string& nombre, int id) {
    auto actual = raiz;

    for (char c : nombre) {
        if (actual->hijos.find(c) == actual->hijos.end()) {
            actual->hijos[c] = std::make_shared<TrieNode>();
        }
        actual = actual->hijos[c];
    }

    actual->esFinal = true;

    if (std::find(actual->ids.begin(), actual->ids.end(), id) == actual->ids.end()) {
        actual->ids.push_back(id);
    }
}

void Trie::eliminar(const std::string& nombre, int id) {
    auto nodo = buscarNodo(nombre);
    if (nodo && nodo->esFinal) {
        auto it = std::find(nodo->ids.begin(), nodo->ids.end(), id);
        if (it != nodo->ids.end()) {
            nodo->ids.erase(it);

            if (nodo->ids.empty()) {
                nodo->esFinal = false;
            }
        }
    }
}

std::vector<int> Trie::buscarExacto(const std::string& nombre) const {
    auto nodo = buscarNodo(nombre);
    if (nodo && nodo->esFinal) {
        return nodo->ids;
    }
    return {};
}

std::vector<std::pair<std::string, std::vector<int>>> Trie::buscarPrefijo(const std::string& prefijo) const {
    std::vector<std::pair<std::string, std::vector<int>>> resultados;

    auto actual = buscarNodo(prefijo);
    if (actual) {
        recorrerTrie(actual, prefijo, resultados);
    }

    return resultados;
}

std::vector<std::string> Trie::autocompletar(const std::string& prefijo, int limite) const {
    std::vector<std::string> resultados;
    int contador = 0;

    auto actual = buscarNodo(prefijo);
    if (actual) {
        recorrerParaAutocompletar(actual, prefijo, resultados, contador, limite);
    }

    return resultados;
}

void Trie::recorrerTrie(const std::shared_ptr<TrieNode>& nodo,
                       const std::string& prefijo,
                       std::vector<std::pair<std::string, std::vector<int>>>& resultados) const {
    if (nodo->esFinal) {
        resultados.emplace_back(prefijo, nodo->ids);
    }

    for (const auto& par : nodo->hijos) {
        recorrerTrie(par.second, prefijo + par.first, resultados);
    }
}

void Trie::recorrerParaAutocompletar(const std::shared_ptr<TrieNode>& nodo,
                                    const std::string& prefijo,
                                    std::vector<std::string>& resultados,
                                    int& contador,
                                    int limite) const {
    if (contador >= limite) return;

    if (nodo->esFinal) {
        resultados.push_back(prefijo);
        contador++;
        if (contador >= limite) return;
    }

    std::vector<char> caracteres;
    for (const auto& par : nodo->hijos) {
        caracteres.push_back(par.first);
    }
    std::sort(caracteres.begin(), caracteres.end());

    for (char c : caracteres) {
        recorrerParaAutocompletar(nodo->hijos.at(c), prefijo + c,
                                 resultados, contador, limite);
        if (contador >= limite) break;
    }
}

std::shared_ptr<TrieNode> Trie::buscarNodo(const std::string& nombre) const {
    auto actual = raiz;

    for (char c : nombre) {
        if (actual->hijos.find(c) == actual->hijos.end()) {
            return nullptr;
        }
        actual = actual->hijos.at(c);
    }

    return actual;
}

// ==================== IMPLEMENTACIONES ARBOL ====================
Arbol::Arbol() : raiz(nullptr), proximoId(2) {
    try {
        raiz = new Nodo(1, "root", CARPETA);
        actualizarMapa(raiz, true);
        actualizarTrie(raiz, true);
        ErrorHandler::mostrarInfo("Arbol inicializado correctamente");
    }
    catch (const std::bad_alloc&) {
        ErrorHandler::mostrarError(FilesystemException("Error de memoria al inicializar el arbol"));
        throw;
    }
}

Arbol::~Arbol() {
    try {
        eliminarRecursivo(raiz);
        for (auto& par : papelera) {
            delete par.second;
        }
        papelera.clear();
        ErrorHandler::mostrarInfo("Arbol y Papelera destruidos correctamente");
    }
    catch (...) {
        ErrorHandler::mostrarAdvertencia("Excepcion no manejada durante la destruccion del arbol");
    }
}

void Arbol::eliminarRecursivo(Nodo* nodo) {
    if (!nodo) return;

    for (Nodo* hijo : nodo->getHijos()) {
        eliminarRecursivo(hijo);
    }
    delete nodo;
}

void Arbol::eliminarFisico(Nodo* nodo) {
    if (!nodo) return;

    for (Nodo* hijo : nodo->getHijos()) {
        eliminarFisico(hijo);
    }

    actualizarTrie(nodo, false);
    actualizarMapa(nodo, false);
    delete nodo;
}

void Arbol::actualizarTrie(Nodo* nodo, bool agregar) {
    if (agregar) {
        trieNombres.insertar(nodo->getNombre(), nodo->getId());
    }
    else {
        trieNombres.eliminar(nodo->getNombre(), nodo->getId());
    }
}

void Arbol::actualizarMapa(Nodo* nodo, bool agregar) {
    if (agregar) {
        mapaNodos[nodo->getId()] = nodo;
    }
    else {
        mapaNodos.erase(nodo->getId());
    }
}

void Arbol::validarCreacionNodo(Nodo* padre, const std::string& nombre) const {
    if (!padre) {
        throw OperacionInvalidaException("crear nodo", "Padre no especificado");
    }

    if (nombre.empty()) {
        throw OperacionInvalidaException("crear nodo", "Nombre no puede estar vacio");
    }

    if (padre->buscarHijoPorNombre(nombre)) {
        throw NombreDuplicadoException(nombre, padre->getId());
    }
}

void Arbol::validarMovimientoNodo(Nodo* origen, Nodo* destino) const {
    if (!origen || !destino) {
        throw OperacionInvalidaException("mover", "Nodo de origen o destino no encontrado");
    }

    if (origen == raiz) {
        throw OperacionInvalidaException("mover", "No se puede mover el nodo raiz");
    }

    if (origen == destino) {
        throw OperacionInvalidaException("mover", "Origen y destino son el mismo nodo");
    }

    if (destino->getTipo() == ARCHIVO) {
        throw DestinoInvalidoException(destino->getId());
    }

    if (destino->buscarHijoPorNombre(origen->getNombre())) {
        throw NombreDuplicadoException(origen->getNombre(), destino->getId());
    }

    Nodo* temp = destino;
    while (temp != nullptr) {
        if (temp == origen) {
            throw OperacionInvalidaException("mover", "No se puede mover un nodo a uno de sus propios descendientes");
        }
        temp = temp->getPadre();
    }
}

void Arbol::validarRenombreNodo(Nodo* nodo, const std::string& nuevoNombre) const {
    if (!nodo) {
        throw OperacionInvalidaException("renombrar", "Nodo no encontrado");
    }

    if (nodo == raiz) {
        throw OperacionInvalidaException("renombrar", "No se puede renombrar el nodo raiz");
    }

    if (nuevoNombre.empty()) {
        throw OperacionInvalidaException("renombrar", "El nuevo nombre no puede estar vacio");
    }

    Nodo* padre = nodo->getPadre();
    if (padre && padre->buscarHijoPorNombre(nuevoNombre)) {
        throw NombreDuplicadoException(nuevoNombre, padre->getId());
    }
}

void Arbol::validarEliminacionNodo(Nodo* nodo) const {
    if (!nodo) {
        throw OperacionInvalidaException("eliminar", "Nodo no encontrado");
    }

    if (nodo == raiz) {
        throw OperacionInvalidaException("eliminar", "No se puede eliminar el nodo raiz");
    }
}

Nodo* Arbol::crearNodo(const std::string& nombre, TipoNodo tipo, const std::string& contenido, Nodo* padre) {
    try {
        if (padre == nullptr) {
            padre = raiz;
        }

        validarCreacionNodo(padre, nombre);

        if (padre->getTipo() == ARCHIVO) {
            throw OperacionInvalidaException("crear", "No se pueden crear nodos dentro de un archivo");
        }

        Nodo* nuevoNodo = new Nodo(proximoId++, nombre, tipo, contenido);
        padre->agregarHijo(nuevoNodo);

        actualizarMapa(nuevoNodo, true);
        actualizarTrie(nuevoNodo, true);

        ErrorHandler::mostrarInfo("Nodo creado: ID " + std::to_string(nuevoNodo->getId()) +
            " ('" + nombre + "') en " + padre->getRutaCompleta());

        return nuevoNodo;
    }
    catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        throw;
    }
}

// Added missing implementations
Nodo* Arbol::getRaiz() const {
    return raiz;
}

void Arbol::establecerProximoId(int nuevoId) {
    proximoId = nuevoId;
}

bool Arbol::eliminarNodo(int id) {
    Nodo* nodo = buscarNodo(id);
    if (!nodo) return false;

    validarEliminacionNodo(nodo);

    Nodo* padre = nodo->getPadre();
    if (padre) {
        padre->eliminarHijo(nodo);
    }

    // Remove recursivamente from structures
    removeFromStructures(nodo);

    addNodoToPapelera(nodo);

    ErrorHandler::mostrarInfo("Nodo ID " + std::to_string(id) + " movido a papelera.");
    return true;
}

void Arbol::removeFromStructures(Nodo* nodo) {  // Helper function
    if (!nodo) return;
    actualizarMapa(nodo, false);
    actualizarTrie(nodo, false);
    for (Nodo* hijo : nodo->getHijos()) {
        removeFromStructures(hijo);
    }
}

bool Arbol::vaciarPapelera() {
    for (auto& par : papelera) {
        eliminarFisico(par.second);
    }
    papelera.clear();
    ErrorHandler::mostrarInfo("Papelera vaciada.");
    return true;
}

Nodo* Arbol::buscarNodo(int id) const {
    auto it = mapaNodos.find(id);
    return it != mapaNodos.end() ? it->second : nullptr;
}

void Arbol::addNodoToPapelera(Nodo* nodo) {
    if (nodo) {
        papelera[nodo->getId()] = nodo;
        nodo->setPadre(nullptr);
    }
}

void Arbol::limpiarArbol() {
    for (Nodo* hijo : raiz->getHijos()) {
        removeFromStructures(hijo);
        eliminarRecursivo(hijo);
    }
    raiz->limpiarHijos();
    mapaNodos.clear();
    trieNombres = Trie();
    actualizarMapa(raiz, true);
    actualizarTrie(raiz, true);
}

void Arbol::agregarNodoCargado(Nodo* nodo) {
    actualizarMapa(nodo, true);
    actualizarTrie(nodo, true);
    // Assume hierarchy already set
}

void Arbol::reconstruirDesdeCarga(Nodo* nodoRaizCargado) {
    // Stub, not used in provided code
}

std::vector<Nodo*> Arbol::recorridoPreorden() const {
    std::vector<Nodo*> resultado;
    recorridoPreordenRecursivo(raiz, resultado);
    return resultado;
}

void Arbol::recorridoPreordenRecursivo(Nodo* nodo, std::vector<Nodo*>& resultado) const {
    if (!nodo) return;
    resultado.push_back(nodo);
    for (Nodo* hijo : nodo->getHijos()) {
        recorridoPreordenRecursivo(hijo, resultado);
    }
}

std::vector<Nodo*> Arbol::getAllNodos() const {
    return recorridoPreorden();
}

int Arbol::getProximoId() const {
    return proximoId;
}

int Arbol::getTamanoPapelera() const {
    return papelera.size();
}

std::vector<Nodo*> Arbol::listarPapelera() const {
    std::vector<Nodo*> result;
    for (const auto& par : papelera) {
        result.push_back(par.second);
    }
    return result;
}

void Arbol::listarHijos(int idPadre) const {
    Nodo* padre = buscarNodo(idPadre);
    if (!padre) throw NodoNotFoundException(idPadre);
    if (padre->getTipo() != CARPETA) throw OperacionInvalidaException("ls", "No es una carpeta");

    std::cout << "Contenido de " << padre->getNombre() << ":\n";
    for (Nodo* hijo : padre->getHijos()) {
        std::cout << hijo->getId() << " - " << hijo->getNombre() << " (" << hijo->getTipoString() << ")\n";
    }
}

// More functions like moverNodo, renombrarNodo, restaurarNodo, buscarNodoPorRuta, buscarPorNombreExacto, etc.
// Add if needed, but for compilation, these are sufficient. Stub example:
bool Arbol::moverNodo(int idOrigen, int idDestino) {
    Nodo* origen = buscarNodo(idOrigen);
    Nodo* destino = buscarNodo(idDestino);
    if (!origen || !destino) return false;
    validarMovimientoNodo(origen, destino);
    Nodo* padre = origen->getPadre();
    if (padre) padre->eliminarHijo(origen);
    destino->agregarHijo(origen);
    return true;
}

bool Arbol::renombrarNodo(int id, const std::string& nuevoNombre) {
    Nodo* nodo = buscarNodo(id);
    if (!nodo) return false;
    validarRenombreNodo(nodo, nuevoNombre);
    nodo->setNombre(nuevoNombre);
    // Update trie if needed, but since name changed, remove old, add new
    actualizarTrie(nodo, false);
    actualizarTrie(nodo, true);
    return true;
}

bool Arbol::restaurarNodo(int id) {
    auto it = papelera.find(id);
    if (it == papelera.end()) return false;
    Nodo* nodo = it->second;
    papelera.erase(it);
    // Assume restore to root or original parent, here to root
    raiz->agregarHijo(nodo);
    addToStructures(nodo);
    return true;
}

void Arbol::addToStructures(Nodo* nodo) {
    if (!nodo) return;
    actualizarMapa(nodo, true);
    actualizarTrie(nodo, true);
    for (Nodo* hijo : nodo->getHijos()) {
        addToStructures(hijo);
    }
}

std::string Arbol::obtenerRutaNodo(int id) const {
    Nodo* nodo = buscarNodo(id);
    if (!nodo) return "";
    return nodo->getRutaCompleta();
}

Nodo* Arbol::buscarNodoPorRuta(const std::string& ruta) const {
    // Stub implementation
    return nullptr;
}

std::vector<Nodo*> Arbol::buscarPorNombreExacto(const std::string& nombre) const {
    std::vector<int> ids = trieNombres.buscarExacto(nombre);
    std::vector<Nodo*> result;
    for (int id : ids) {
        Nodo* nodo = buscarNodo(id);
        if (nodo) result.push_back(nodo);
    }
    return result;
}

std::vector<std::pair<std::string, std::vector<int>>> Arbol::buscarPorPrefijo(const std::string& prefijo) const {
    return trieNombres.buscarPrefijo(prefijo);
}

std::vector<std::string> Arbol::autocompletarNombres(const std::string& prefijo, int limite) const {
    return trieNombres.autocompletar(prefijo, limite);
}

int Arbol::getAltura() const {
    // Stub
    return 0;
}

int Arbol::getTamano() const {
    return mapaNodos.size();
}

bool Arbol::existeNodo(int id) const {
    return mapaNodos.find(id) != mapaNodos.end();
}

bool Arbol::existeNombreEnUbicacion(const std::string& nombre, int idPadre) const {
    Nodo* padre = buscarNodo(idPadre);
    return padre && padre->buscarHijoPorNombre(nombre);
}

// ==================== GESTOR JSON ====================
std::string GestorJSON::escapeJson(const std::string& str) {
    std::string result;
    for (char c : str) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\t': result += "\\t"; break;
            default: result += c; break;
        }
    }
    return result;
}

std::string GestorJSON::unescapeJson(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            ++i;
            switch (str[i]) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                default: result += str[i]; break;
            }
        } else {
            result += str[i];
        }
    }
    return result;
}

bool GestorJSON::guardarArbol(const Arbol& arbol, const std::string& nombreArchivo) {
    std::ofstream archivo(nombreArchivo);
    if (!archivo) {
        throw ArchivoIOException(nombreArchivo, "escritura");
    }

    archivo << "{\n  \"proximo_id\": " << arbol.getProximoId() << ",\n  \"nodos\": [\n";
    std::vector<Nodo*> nodos = arbol.getAllNodos();
    for (size_t i = 0; i < nodos.size(); ++i) {
        Nodo* nodo = nodos[i];
        archivo << "    {\n      \"id\": " << nodo->getId() << ",\n      \"nombre\": \"" << escapeJson(nodo->getNombre()) << "\",\n      \"tipo\": \"" << nodo->getTipoString() << "\",\n      \"contenido\": \"" << escapeJson(nodo->getContenido()) << "\",\n      \"padre_id\": " << (nodo->getPadre() ? nodo->getPadre()->getId() : 0) << "\n    }";
        if (i < nodos.size() - 1) archivo << ",\n";
        else archivo << "\n";
    }
    archivo << "  ]\n}";
    archivo.close();
    ErrorHandler::mostrarInfo("Arbol guardado en '" + nombreArchivo + "'");
    return true;
}

bool GestorJSON::cargarArbol(Arbol& arbol, const std::string& nombreArchivo) {
    try {
        std::ifstream archivo(nombreArchivo);
        if (!archivo) {
            throw ArchivoIOException(nombreArchivo, "lectura");
        }

        std::string json_str((std::istreambuf_iterator<char>(archivo)), std::istreambuf_iterator<char>());
        archivo.close();

        if (json_str.empty()) {
            throw ArchivoVacioException(nombreArchivo);
        }

        // Parse proximo_id
        size_t pos_proximo = json_str.find("\"proximo_id\"");
        if (pos_proximo == std::string::npos) {
            throw FormatoJSONException("No se encontró 'proximo_id'");
        }
        size_t colon_pos = json_str.find(':', pos_proximo);
        size_t comma_pos = json_str.find(',', colon_pos);
        std::string id_str = json_str.substr(colon_pos + 1, comma_pos - colon_pos - 1);
        id_str.erase(std::remove_if(id_str.begin(), id_str.end(), ::isspace), id_str.end());
        int proximo_id_cargado = std::stoi(id_str);

        // Parse nodos array
        size_t pos_nodos = json_str.find("\"nodos\"");
        if (pos_nodos == std::string::npos) {
            throw FormatoJSONException("No se encontró 'nodos'");
        }
        size_t bracket_pos = json_str.find('[', pos_nodos);
        if (bracket_pos == std::string::npos) {
            throw FormatoJSONException("No se encontró array de nodos");
        }

        std::vector<std::string> nodos_json;
        std::string current_nodo;
        bool in_string = false;
        bool escape = false;
        int brace_count = 0;
        size_t i = bracket_pos + 1;
        while (i < json_str.size()) {
            char c = json_str[i];
            if (escape) {
                current_nodo += c;
                escape = false;
            } else if (c == '\\') {
                current_nodo += c;
                escape = true;
            } else if (c == '"') {
                current_nodo += c;
                in_string = !in_string;
            } else if (!in_string) {
                if (c == '{') {
                    brace_count++;
                    if (brace_count == 1) {
                        current_nodo = "{";
                    } else {
                        current_nodo += c;
                    }
                } else if (c == '}') {
                    brace_count--;
                    current_nodo += c;
                    if (brace_count == 0) {
                        nodos_json.push_back(current_nodo);
                        current_nodo.clear();
                    }
                } else if (c == ']' && brace_count == 0) {
                    break;
                } else {
                    current_nodo += c;
                }
            } else {
                current_nodo += c;
            }
            i++;
        }

        arbol.limpiarArbol();
        arbol.establecerProximoId(proximo_id_cargado);

        std::map<int, Nodo*> nodos_creados;
        std::map<int, int> padre_ids;
        std::vector<int> nodos_papelera;

        for (const std::string& nodo_json : nodos_json) {
            if (nodo_json.empty()) continue;

            int id = 0;
            std::string nombre;
            std::string tipo_str;
            std::string contenido_nodo;
            int padre_id = 0;

            // Parse id
            size_t id_pos = nodo_json.find("\"id\"");
            if (id_pos != std::string::npos) {
                size_t colon_pos = nodo_json.find(':', id_pos);
                size_t comma_pos = nodo_json.find(',', colon_pos);
                std::string id_str = nodo_json.substr(colon_pos + 1, comma_pos - colon_pos - 1);
                id_str.erase(std::remove_if(id_str.begin(), id_str.end(), ::isspace), id_str.end());
                id = std::stoi(id_str);
            }

            if (id == 0) {
                throw FormatoJSONException("Nodo sin ID o ID inválido");
            }

            // Parse nombre
            size_t nombre_pos = nodo_json.find("\"nombre\"");
            if (nombre_pos != std::string::npos) {
                size_t quote_pos = nodo_json.find('"', nombre_pos + 9);  // After "nombre":
                if (quote_pos != std::string::npos) {
                    size_t end_quote = nodo_json.find('"', quote_pos + 1);
                    if (end_quote != std::string::npos) {
                        nombre = nodo_json.substr(quote_pos + 1, end_quote - quote_pos - 1);
                        nombre = unescapeJson(nombre);
                    }
                }
            }

            // Parse tipo
            size_t tipo_pos = nodo_json.find("\"tipo\"");
            if (tipo_pos != std::string::npos) {
                size_t quote_pos = nodo_json.find('"', tipo_pos + 7);
                if (quote_pos != std::string::npos) {
                    size_t end_quote = nodo_json.find('"', quote_pos + 1);
                    if (end_quote != std::string::npos) {
                        tipo_str = nodo_json.substr(quote_pos + 1, end_quote - quote_pos - 1);
                    }
                }
            }

            // Parse contenido
            size_t contenido_pos = nodo_json.find("\"contenido\"");
            if (contenido_pos != std::string::npos) {
                size_t quote_pos = nodo_json.find('"', contenido_pos + 11);
                if (quote_pos != std::string::npos) {
                    size_t end_quote = nodo_json.find('"', quote_pos + 1);
                    if (end_quote != std::string::npos) {
                        contenido_nodo = nodo_json.substr(quote_pos + 1, end_quote - quote_pos - 1);
                        contenido_nodo = unescapeJson(contenido_nodo);
                    }
                }
            }

            // Parse padre_id
            size_t padre_pos = nodo_json.find("\"padre_id\"");
            if (padre_pos != std::string::npos) {
                size_t colon_pos = nodo_json.find(':', padre_pos);
                size_t comma_pos = nodo_json.find(',', colon_pos);
                size_t end_pos = nodo_json.find('}', colon_pos);
                size_t final_pos = (comma_pos != std::string::npos && comma_pos < end_pos) ? comma_pos : end_pos;
                std::string padre_str = nodo_json.substr(colon_pos + 1, final_pos - colon_pos - 1);
                padre_str.erase(std::remove_if(padre_str.begin(), padre_str.end(), ::isspace), padre_str.end());
                padre_id = std::stoi(padre_str);
            }

            TipoNodo tipo = (tipo_str == "CARPETA") ? CARPETA : ARCHIVO;

            Nodo* nodo;
            if (id == 1) {
                nodo = arbol.getRaiz();
                if (nodo) {
                    nodo->setNombre(nombre);
                    nodos_creados[id] = nodo;
                }
            } else {
                nodo = new Nodo(id, nombre, tipo, contenido_nodo);
                nodos_creados[id] = nodo;
            }

            padre_ids[id] = padre_id;

            if (padre_id < 0) {
                nodos_papelera.push_back(id);
            }
        }

        for (auto& par : nodos_creados) {
            int id = par.first;
            if (id == 1) continue;

            int padre_id = padre_ids[id];

            if (padre_id > 0) {
                auto it_padre = nodos_creados.find(padre_id);
                if (it_padre != nodos_creados.end()) {
                    try {
                        it_padre->second->agregarHijo(par.second);
                    } catch (const std::exception& e) {
                        ErrorHandler::mostrarAdvertencia("Error al agregar hijo " +
                            std::to_string(id) + " a padre " + std::to_string(padre_id) +
                            ": " + e.what());
                        nodos_papelera.push_back(id);
                    }
                } else {
                    ErrorHandler::mostrarAdvertencia("Padre ID " +
                        std::to_string(padre_id) + " no encontrado para nodo " +
                        std::to_string(id) + ". Moviendo a papelera.");
                    nodos_papelera.push_back(id);
                }
            }
        }

        for (auto& par : nodos_creados) {
            int id = par.first;
            Nodo* nodo = par.second;

            if (id != 1) {
                bool en_papelera = std::find(nodos_papelera.begin(), nodos_papelera.end(), id) != nodos_papelera.end();

                if (!en_papelera) {
                    arbol.agregarNodoCargado(nodo);
                }
            }
        }

        for (int id_papelera : nodos_papelera) {
            auto it = nodos_creados.find(id_papelera);
            if (it != nodos_creados.end()) {
                Nodo* nodo = it->second;

                if (nodo->getPadre()) {
                    nodo->getPadre()->eliminarHijo(nodo);
                }

                arbol.addNodoToPapelera(nodo);
            }
        }

        ErrorHandler::mostrarInfo("Arbol cargado desde '" + nombreArchivo + "' con " +
            std::to_string(nodos_json.size()) + " nodos. Proximo ID: " + std::to_string(proximo_id_cargado));

        return true;
    } catch (const FilesystemException& e) {
        ErrorHandler::mostrarError(e);
        return false;
    } catch (const std::exception& e) {
        ErrorHandler::mostrarError(FilesystemException(std::string("Error al cargar JSON: ") + e.what()));
        return false;
    }
}