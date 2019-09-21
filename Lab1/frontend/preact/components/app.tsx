import { h, Component } from 'preact';
import { Grid, Cell, Card } from 'preact-fluid';

import Header from './header/header';
import ComponentGrid from './componentGrid/componentGrid';

interface MyProps {}
interface MyState {}

export default class App extends Component<MyProps, MyState> {
    /** Gets fired when the route changes.
     *	@param {Object} event		"change" event from [preact-router](http://git.io/preact-router)
     *	@param {string} event.url	The newly routed URL
     */
    currentUrl;

    handleRoute = e => {
        this.currentUrl = e.url;
    };
    // <GridRow extraSmall="10" small="10" medium="10" large="10">
    // </GridRow>

    render() {
        return (
            <div id="app">
                <Header />
                <ComponentGrid />
            </div>
        );
    }
}
