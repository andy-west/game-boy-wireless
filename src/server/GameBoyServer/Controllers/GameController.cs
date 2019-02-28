using System;
using System.Linq;
using Microsoft.AspNetCore.Mvc;

namespace GameBoyServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class GameController : ControllerBase
    {
        private readonly IGame _game;

        public GameController(IGame game)
        {
            _game = game;
        }

        [HttpGet("saysomething/{text}")]
        public string SaySomething(string text)
        {
            return text;
        }

        [HttpGet("join/{gameBoyId}")]
        public string Join(int gameBoyId)
        {
            PlayerColor playerColor;

            var players = _game.Players;

            switch (players.Count)
            {
                case 0:
                    playerColor = PlayerColor.Black;
                    _game.Players.Add(new Player(gameBoyId, playerColor));

                    break;
                case 1:
                    if (players.Single().GameBoyId == gameBoyId)
                    {
                        playerColor = PlayerColor.Black;
                    }
                    else
                    {
                        playerColor = PlayerColor.White;
                        _game.Players.Add(new Player(gameBoyId, playerColor));

                        _game.State = GameState.BlackTurn;
                    }

                    break;
                case 2:
                    _game.Reset();
                    playerColor = PlayerColor.Black;
                    _game.Players.Add(new Player(gameBoyId, playerColor));

                    break;
                default:
                    throw new Exception("Invalid number of players.");
            }

            return playerColor.ToString();
        }

        [HttpGet("getplayercount")]
        public int GetPlayerCount()
        {
            return _game.Players.Count;
        }

        [HttpGet("placedisc/{gameBoyId}/{discIndex}")]
        public PlaceDiscResult TryPlaceDisc(int gameBoyId, int discIndex)
        {
            var player = _game.Players.Single(p => p.GameBoyId == gameBoyId);
            var x = discIndex % 8;
            var y = (discIndex - x) / 8;
            var result = _game.Board.TryPlaceDisc(player.Color, x, y);

            return result;
        }

        [HttpGet("getlastmove")]
        public PlaceDiscResult GetLastMove()
        {
            return _game.Board.LastMove;
        }
    }
}
