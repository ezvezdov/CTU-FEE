package cz.cvut.fel.pjv;

public class BruteForceAttacker extends Thief {

    private char[] symbolsSet;
    private int sizeOfPassword;

    private char[] makePassword(char[] password, int currentSize){
        if(sizeOfPassword == currentSize && tryOpen(password)){
            return password;
        }
        if(sizeOfPassword == currentSize){
            return null;
        }

        char[] newPassword;

        for(int i = 0; i < symbolsSet.length; i++){
            password[currentSize] = symbolsSet[i];
            newPassword = makePassword(password, currentSize+1);
            if(newPassword != null){
                return newPassword;
            }
        }
        return null;

    }

    @Override
    public void breakPassword(int sizeOfPassword) {
        this.sizeOfPassword = sizeOfPassword;
        this.symbolsSet = getCharacters();
        char[] password = new char[sizeOfPassword];
        password = makePassword(password, 0);

    }
}
