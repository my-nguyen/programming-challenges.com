import java.io.FileNotFoundException;


class Command {
    public char   _command;
    public int    _x1;
    public int    _x2;
    public int    _y1;
    public int    _y2;
    public char   _color;
    public String _filename;

    private static Image _image = new Image();


    public void execute() throws FileNotFoundException {

        if (Debug._level >= Debug.COPIOUS)
            System.out.print("Command " + _command + "\n");

        switch (_command)
        {
        case 'I':
            _image.create(_x1, _y1);
            break;
        case 'C':
            _image.clear();
            break;
        case 'L':
            _image.color(_x1, _y1, _color);
            break;
        case 'V':
            _image.vertical(_x1, _y1, _y2, _color);
            break;
        case 'H':
            _image.horizontal(_x1, _x2, _y1, _color);
            break;
        case 'K':
            _image.rectangle(_x1, _y1, _x2, _y2, _color);
            break;
        case 'F':
            _image.fill(_x1, _y1, _image.get(_x1, _y1), _color);

            // debug print is done outside Image::fill(), unlike other Image
            // methods, because Image::fill() is recursive
            if (Debug._level >= Debug.LIGHT)
                System.out.print("F " + _x1 + " " + _y1 +
                                 " " + _color + "\n" + _image);
            break;
        case 'S':
            _image.save(_filename);
            break;
        case 'X':
            break;
        }
    }
}
