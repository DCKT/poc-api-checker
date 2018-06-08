let checkStruct = (data, decoder) =>
  data |> Json.stringify |> Json.parseOrRaise |> decoder |> ignore;

let check = (name, promise, decoder) =>
  promise
  |> Js.Promise.then_(response => {
       checkStruct(response##data, decoder);
       Js.Promise.resolve();
     })
  |> Js.Promise.catch(error => {
       Js.log("Endpoint " ++ name);
       Js.log(error);
       Js.Promise.resolve();
     })
  |> ignore;