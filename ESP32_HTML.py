from os import path

####################### UTILITY FUNCTIONS #######################
# Takes in a file path, and returns the text 
# content without newline characters.
def loadFile(file_path):
    esp_compatible_text = ""

    with open(file_path, "r") as file:
        for line in file.read().splitlines():
            esp_compatible_text += formatForESP(line)
    
    return esp_compatible_text

# Formats the html code to be in the following 
# format: p += "<h2 class\"SOME_CLASS\">TEXT</h2>"
def formatForESP(html_line):
    text = html_line.strip("\n")
    text = html_line.replace('"', '\"')
    text = html_line.replace("'", "\'")
    text = f'  ptr += "{text}\\n";\n'
    return text

#################################################################

# Takes in an HTML_FILE_PATH and returns the 
# HTML in an ESP32 compatible way.
def makeESPCompatible(HTML_FILE_PATH):
    # Current Python file location within the system.
    # CURRENT_LOCATION = path.dirname(path.realpath(__file__))
    # Base HTML template.
    # HTML_FILE_PATH = f"{CURRENT_LOCATION}\\index.html"

    esp_compatible_html = loadFile(HTML_FILE_PATH)

    # Outputs the ESP32 compatible file.
    # with open('sample.txt', 'w') as output_file:
    #     output_file.write(esp_compatible_html)
    
    return esp_compatible_html