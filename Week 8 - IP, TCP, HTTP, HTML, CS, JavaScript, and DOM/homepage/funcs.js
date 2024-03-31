function getexp(event)
{
    //button details
    let button = event.target;

    var exp = button.innerHTML;
    //console.log(exp);


    document.getElementById("exptitle").innerHTML = exp;
    if (button.id == "itworx")
    {
        document.getElementById("expperiod").innerHTML = "(Jul 2022 – Now)";
        //show and highlight itworx
        document.getElementById('itworxd').style.display = 'table-row';
        document.getElementById('itworxb').style.backgroundColor = 'white';


        //hide others #212529
        document.getElementById('basatad').style.display = "none";
        document.getElementById('azdand').style.display = "none";
        document.getElementById('nbed').style.display = "none";

        //
    }
    else if (button.id == "basata")
    {
        document.getElementById("expperiod").innerHTML = "(Jul 2020 – Jul 2022)";
        //show basata
        document.getElementById('basatad').style.display = 'table-row';
        document.getElementById('basatab').style.backgroundColor = 'white';

        //hide others
        document.getElementById('itworxd').style.display = "none";
        document.getElementById('azdand').style.display = "none";
        document.getElementById('nbed').style.display = "none";

        //background color for others
        document.getElementById('itworxb').style.backgroundColor = "#212529";
        document.getElementById('azdanb').style.backgroundColor = "#212529";
        document.getElementById('nbeb').style.backgroundColor = "#212529";

    }
    else if (button.id == "azdan")
    {
        document.getElementById("expperiod").innerHTML = "(Jun 2019 - May 2020)";
        //show azdan
        document.getElementById('azdand').style.display = 'table-row';
        document.getElementById('azdanb').style.backgroundColor = 'white';


        //hide others
        document.getElementById('basatad').style.display = "none";
        document.getElementById('itworxd').style.display = "none";
        document.getElementById('nbed').style.display = "none";

        //background color for others
        document.getElementById('itworxb').style.backgroundColor = "#212529";
        document.getElementById('basatab').style.backgroundColor = "#212529";
        document.getElementById('nbeb').style.backgroundColor = "#212529";
    }
    else if (button.id == "nbe")
    {
        document.getElementById("expperiod").innerHTML = "(May 2015 – Jun 2019)";
        //show nbe
        document.getElementById('nbed').style.display = 'table-row';
        document.getElementById('nbeb').style.backgroundColor = 'white';


        //hide others
        document.getElementById('basatad').style.display = "none";
        document.getElementById('itworxd').style.display = "none";
        document.getElementById('azdand').style.display = "none";

        //background color for others
        document.getElementById('itworxb').style.backgroundColor = "#212529";
        document.getElementById('azdanb').style.backgroundColor = "#212529";
        document.getElementById('basatab').style.backgroundColor = "#212529";
    }
}



function getcert(event)
{
    //button details
    let button = event.target;

    var exp = button.innerHTML;
    //console.log(exp);

    document.getElementById("certitle").innerHTML = exp;
    if (button.id == "bi")
    {
        document.getElementById("cerperiod").innerHTML = "(Sep 2023 – Sep 2024)";
        //show bi
        document.getElementById('bid').style.display = 'table-row';
        document.getElementById('bib').style.backgroundColor = 'white';


        //hide others
        document.getElementById('mld').style.display = "none";
        document.getElementById('cfad').style.display = "none";
        document.getElementById('dad').style.display = "none";

        //color others
        document.getElementById('mlb').style.backgroundColor = "#212529";
        document.getElementById('cfab').style.backgroundColor = "#212529";
        document.getElementById('dab').style.backgroundColor = "#212529";
    }
    else if (button.id == "ml")
    {
        document.getElementById("cerperiod").innerHTML = "(Jul 2020 – Jul 2022)";
        //show ml
        document.getElementById('mld').style.display = 'table-row';
        document.getElementById('mlb').style.backgroundColor = 'white';

        //hide others
        document.getElementById('dad').style.display = "none";
        document.getElementById('cfad').style.display = "none";
        document.getElementById('bid').style.display = "none";

        //color others
        document.getElementById('dab').style.backgroundColor = "#212529";
        document.getElementById('cfab').style.backgroundColor = "#212529";
        document.getElementById('bib').style.backgroundColor = "#212529";
    }
    else if (button.id == "cfa")
    {
        document.getElementById("cerperiod").innerHTML = "(Jun 2019 - May 2020)";
        //show cfa
        document.getElementById('cfad').style.display = 'table-row';
        document.getElementById('cfab').style.backgroundColor = 'white';

        //hide others
        document.getElementById('bid').style.display = "none";
        document.getElementById('dad').style.display = "none";
        document.getElementById('mld').style.display = "none";

        //color others
        document.getElementById('bib').style.backgroundColor = "#212529";
        document.getElementById('dab').style.backgroundColor = "#212529";
        document.getElementById('mlb').style.backgroundColor = "#212529";
        }
    else if (button.id == "da")
    {
        document.getElementById("cerperiod").innerHTML = "(May 2015 – Jun 2019)";
        //show data
        document.getElementById('dad').style.display = 'table-row';
        document.getElementById('dab').style.backgroundColor = 'white';

        //hide others
        document.getElementById('bid').style.display = "none";
        document.getElementById('mld').style.display = "none";
        document.getElementById('cfad').style.display = "none";

        //color others
        document.getElementById('bib').style.backgroundColor = "#212529";
        document.getElementById('mlb').style.backgroundColor = "#212529";
        document.getElementById('cfab').style.backgroundColor = "#212529";
    }
}
