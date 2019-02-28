using System.Diagnostics;
using Microsoft.AspNetCore.Mvc;

namespace GameBoyServer.Controllers
{
    [Route("api/[controller]")]
    [ApiController]
    public class ValuesController : ControllerBase
    {
        [HttpGet]
        public ActionResult<string> Get()
        {
            const string hello = "Hello, world!";
            Debug.WriteLine(hello);
            return hello;
        }
    }
}
