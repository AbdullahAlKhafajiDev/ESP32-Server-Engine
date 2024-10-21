from os import path
import json
from pathlib import Path
from ESP32_HTML import makeESPCompatible

####################### UTILITY FUNCTIONS #######################
# Takes in a file path, and returns the text 
# content without newline characters.
def loadFile(file_path):
    text_file_with_newline = Path(file_path).read_text()
    text_file = text_file_with_newline.replace("\n", "")
    return text_file

# Takes in an HTML file path, and returns 
# the HTML code as <String>.
def loadCCode(c_file_path):
    c_file = Path(c_file_path).read_text()
    return c_file

# Takes in a dictionary that has ONE key:value pair.
# and returns the key as <String>.
def getKey(dictionary):
    keys = list(dictionary.keys())
    if (len(keys) == 1):
        return keys[0]
    else:
        ("Misuse of function getKeys()\n\t Dictionary:\n", dictionary)

#################################################################

TEMPLATE_FUNCTION = '''
void handle_BACKEND_VALUE_CONFIG()
{
  digitalWrite(PIN, STATUS);
  server.send(200, "json/application", "{\"message\":\"Server: SERVER_RESPONSE\"}");
}
'''

# Retrieves the backend values that the backend needs to process.
def getBackendValues(form_elements_array):
    backend_values = []

    for form_element in form_elements_array:
        form_element_name = getKey(form_element)
        
        if ('input_name' in form_element[form_element_name]):
            backend_values.append(form_element[form_element_name]['input_name'])

    return backend_values

# Returns <String> backend function handlers.
def makeBackendHandlers(backend_values):
    backend_handler_functions = ""

    for backend_value in backend_values:
        backend_handler_functions += TEMPLATE_FUNCTION.replace("BACKEND_VALUE_CONFIG", backend_value)
        backend_handler_functions += "\n"

    return backend_handler_functions

# Returns an IF statement that ensures all args are passed in.
def makeIfStatement(backend_values):
    SERVER_HAS_TEMPLATE = 'server.hasArg("BACKEND_VALUE")'

    if_statement = ""
    if_statement += SERVER_HAS_TEMPLATE.replace("BACKEND_VALUE", backend_values[0])

    index = 1
    if (len(backend_values) > 1):
        while (index < len(backend_values)):
            if_statement += f" && {SERVER_HAS_TEMPLATE.replace("BACKEND_VALUE", backend_values[index])}"
            index += 1

    return if_statement

def addPasswordChecker(backend_values):
    PASSWORD_IF_STATEMENT = '''if (password_field == "<PASSWORD>")
    {
        BACKEND_VALUES_HANDLERS
    } else
    {
        server.send(403, "text/plain", "Forbidden: Invalid password");
        return;
    }
'''

    if ("password" in backend_values):
        return PASSWORD_IF_STATEMENT
    
def varsInitializers(backend_values):
    INITIALIZER_TEMPLATE = f'String BACKEND_VALUE_field = server.arg("BACKEND_VALUE");\n'
    initializer_list = ""

    for backend_value in backend_values:
        initializer_list += INITIALIZER_TEMPLATE.replace("BACKEND_VALUE", backend_value)

    return initializer_list

def main():
    # Current Python file location within the system.
    CURRENT_LOCATION = path.dirname(path.realpath(__file__))
    # Base HTML template.
    HTML_FILE_PATH = f"{CURRENT_LOCATION}\\index.html"
    # config.json path.
    JSON_CONFIG_PATH = f"{CURRENT_LOCATION}\\config.json"
    # Base HTML template.
    BASE_TEMPLATE_PATH = f"{CURRENT_LOCATION}\\esp32_sketch_TEMPLATE.ino"

    c_code = loadCCode(BASE_TEMPLATE_PATH)
    # ESP32 compatible HTML.
    espHTML = makeESPCompatible(HTML_FILE_PATH)

    c_code = c_code.replace("HTML_CONFIG", espHTML)    

    # loads json.config
    config_json = json.loads(loadFile(JSON_CONFIG_PATH))

    backend_values = getBackendValues(config_json['form_elements'])

    backend_handler_functions = makeBackendHandlers(backend_values)
    c_code = c_code.replace("BACKEND_HANDLER_FUNCTIONS_CONFIG", backend_handler_functions)

    if_statement = makeIfStatement(backend_values)
    c_code = c_code.replace("ARGS_ASSURANCE_IF_STATEMENT_CONFIG", if_statement)

    vars_initializers = varsInitializers(backend_values)
    c_code = c_code.replace("VARS_INITIALIZER_LIST_CONFIG", vars_initializers)

    password_check = addPasswordChecker(backend_values)
    c_code = c_code.replace("BACKEND_VALUES_HANDLERS_CONFIG", password_check)

    with open("esp32_server.ino", "w") as output_file:
        output_file.write(c_code)


if __name__ == "__main__":
    main()