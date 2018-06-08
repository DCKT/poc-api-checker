let checkStruct = (data: Js.Json.t, decoder: Json.Decode.decoder('a)) =>
  data |> Json.stringify |> Json.parseOrRaise |> decoder |> ignore;

type axiosResponse = {. "data": Js.Json.t};

let check = (~label: string, ~request, ~decoder: Json.Decode.decoder('a)) =>
  request
  |> Js.Promise.then_(response => {
       checkStruct(response##data, decoder);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error => {
       Js.log("\n ===> Endpoint " ++ label);
       Js.log(error);
       Js.Promise.resolve();
     })
  |> ignore;