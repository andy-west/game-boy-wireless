namespace GameBoyServer
{
    public class PlaceDiscResult
    {
        public PlayerColor PlayerColor { get; }
        public int X { get; }
        public int Y { get; }
        public bool IsMoveValid { get; }
        public int[][] DiscsToFlip { get; }

        public PlaceDiscResult(PlayerColor playerColor, int x, int y, bool isMoveValid, int[][] discsToFlip)
        {
            PlayerColor = playerColor;
            X = x;
            Y = y;
            IsMoveValid = isMoveValid;
            DiscsToFlip = discsToFlip;
        }
    }
}
