class Checker {
    // OPEN    means the current square is empty
    // BLOCKED means the current square is occupied by a chess piece, but since
    //         the piece is either a friendly or an unharmful hostile, the
    //         current king is not under attacked, but cannot go further
    // CHECKED means the current square is occupied by a hostile piece that is
    //         putting the current king under attack or in check
    enum Status { OPEN, BLOCKED, CHECKED };


    Status   _status;
    char     _id;
    Location _location;


    Checker() {
        _status = Status.OPEN;
    }

    void set(char id, int row, int column) {
        _status = Status.CHECKED;
        _id = id;
        _location = new Location(row, column);
    }
}
