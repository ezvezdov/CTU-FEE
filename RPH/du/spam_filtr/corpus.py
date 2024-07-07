import os


'''
Třída Corpus (v modulu corpus.py) bude obalovat adresář s emaily a umožní nám je snadno procházet. Třída bude mít následující vlastnosti:

Při inicializaci jí bude předána cesta k adresáři s emaily.
Třída bude mít metodu emails(), která bude generátorem.Tato metoda si bude vědoma toho, že v adresáři s 
emaily mohou být i soubory s metainformacemi. Název těchto souborů bude vždy začínat znakem ! 
(např. !truth.txt), proto všechny soubory začínající vykřičníkem v této metodě ignorujte!!!
'''
class Corpus:
    def __init__(self,fpath):
        self.fpath = fpath
    def emails(self):
        files = os.listdir(self.fpath)
        for filename in files:
            if(filename[0] == '!'):
                continue

            current_file = open(os.path.join(self.fpath,filename),"r",encoding="utf-8")
            file_body  = current_file.read()
            current_file.close()

            yield filename, file_body